import numpy as np

INPUT_PATH = "/Users/vincentmayer/repos/AdventOfCode/01/input.txt"


with open(INPUT_PATH, "r") as file:
    lines = file.readlines()
    list_a = [int(line.strip().split(" ")[0]) for line in lines]
    list_b = [int(line.strip().split(" ")[-1]) for line in lines]

list_a = sorted(list_a)
list_b = sorted(list_b)

diff = np.abs(np.array(list_a) - np.array(list_b))

print("Part 1: ", sum(diff))

appearances = {elem: list_b.count(elem) for elem in set(list_a)}

print("Part 2: ", sum(appearances.get(elem, 0) * elem for elem in list_a))
