local ffi = require "ffi"
local bit = require "bit"

local ext = ({Windows = 'dll', Linux = 'so', OSX = 'dylib'})[ffi.os]

if not ext then
  error("Unknown operating system: "..ffi.os)
end

local unboxed = "native/unboxed/libmgba."..ext

if not love.filesystem.exists(unboxed) then
  print("Unboxing native library...")

  love.filesystem.createDirectory("native/unboxed")
  local src, err = love.filesystem.newFile("native/"..ffi.arch.."/libmgba."..ext, "r")
  if not src then
    error("Unsupported arch: "..ffi.arch)
  end
  local dest = love.filesystem.newFile(unboxed, "w")
  while true do
    local data = src:read(4096)
    if #data == 0 or not data then break end
    dest:write(data)
  end
  src:close()
  dest:close()
end

local native = ffi.load(love.filesystem.getSaveDirectory()..'/'..unboxed)
_MGBA_NATIVE = native -- Keep a global reference to prevent the native library from being unloaded under us

do
  local def = love.filesystem.read("mgba.h")
  ffi.cdef(def)
end

local game = ffi.new("const char *", _GAME)
local core = native.mCoreFind(game)
assert(core:init(), "Core failed to init")
native.mCoreInitConfig(core, "love")
native.mCoreLoadConfig(core)
assert(native.mCoreLoadFile(core, game), "Game failed to load")

local resolution = ffi.new("unsigned[2]")
core:desiredVideoDimensions(resolution, resolution+1)
local width, height = resolution[0], resolution[1]
local buffer = ffi.new("color_t[?]", width*height)
core:setVideoBuffer(buffer, width)
local imagedata = love.image.newImageData(width, height)
local image = love.graphics.newImage(imagedata)

core:reset()

local inputKeys = {}
local inputInfo = {
  nKeys = 10,
  keyId = {
[0]="A",
		"B",
		"Select",
		"Start",
		"Right",
		"Left",
		"Up",
		"Down",
		"R",
		"L"
  }
}
for i=0, inputInfo.nKeys-1 do
  inputKeys[ffi.string(inputInfo.keyId[i]):lower()] = bit.lshift(1, i)
end

local keymap = {
  up = inputKeys.up,
  down = inputKeys.down,
  left = inputKeys.left,
  right = inputKeys.right,
  x = inputKeys.a,
  z = inputKeys.b,
  ["return"] = inputKeys.start,
  rshift = inputKeys.select,
  a = inputKeys.l,
  s = inputKeys.r
}

function love.draw()
  core:runFrame()

  -- This version of Love2D doesn't support format types in imagedata
  -- So we have to copy each pixel manually and set the alpha to 255... bummer...
  local imagedataBuffer = ffi.cast("color_t*", imagedata:getPointer())
  for i=1, width*height do
    imagedataBuffer[i] = bit.bor(0xFF000000, buffer[i])
  end
  image:refresh()

  love.graphics.setColor(255, 255, 255)
  love.graphics.draw(image)
end

function love.keypressed(key)
  local kid = keymap[key]
  if kid then
    core:addKeys(kid)
  end
end

function love.keyreleased(key)
  local kid = keymap[key]
  if kid then
    core:clearKeys(kid)
  end
end
