local timeTaken = 0
local function measure()
    local t = os.clock() * 1000
    print(t - timeTaken)
    timeTaken = t
end

local F = io.open(select(1, ...)) or io.stdin

local outOfRange = { v = -1 }

local field = setmetatable({}, {
    __index  = function(t,k) return outOfRange end
})

local notDone = {}

local function GetCoords(x,y)
    return x * 1024 + y
end

local line = F:read()
local y = 1
while line do
    if line ~= "" then
        for x = 1, #line do
            local t = {
                -- store AASCII value
                v = line:byte(x),
                x = x,
                y = y,
            }
            field[GetCoords(x,y)] = t
            notDone[t] = true
        end
    end
    line = F:read()
    y = y + 1
end

local dirs = {
    { x = 1, y = 0 },
    { x = -1, y = 0 },
    { x = 0, y = 1 },
    { x = 0, y = -1 },
}

local function GetArea(sol, spot)
    notDone[spot] = nil
    sol.area = sol.area + 1
    for id, dir in ipairs(dirs) do
        local neigh = field[GetCoords(spot.x + dir.x, spot.y + dir.y)]
        if neigh.v == spot.v then
            if notDone[neigh] then
                GetArea(sol, neigh)
            end
        else
            sol.perim = sol.perim + 1
            local k = tostring(id) .. " "
            local v
            if dir.x == 0 then
                k = k .. tostring(spot.y)
                v = spot.x
            else
                k = k .. tostring(spot.x)
                v = spot.y
            end
            local t = sol.perims[k] or {}
            t[#t+1] = v
            sol.perims[k] = t
        end
    end
end

local p1 = 0
local p2 = 0

while next(notDone) do
    local area = 0
    local perim = 0
    local t = next(notDone)
    local sol = { area = 0, perim = 0, perims = {} }
    GetArea(sol, t)
    p1 = p1 + sol.area * sol.perim
    local p = sol.perim
    for k, perims in pairs(sol.perims) do
        table.sort(perims)
        for i = 1, #perims - 1 do
            if perims[i] == perims[i+1] - 1 then
                p = p - 1
            end
        end
    end
    p2 = p2 + sol.area * p
end

print(p1, p2)

measure()
