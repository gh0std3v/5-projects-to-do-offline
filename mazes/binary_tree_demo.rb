# @Author: S. Sharma <silentcat>
# @Date:   2019-11-24T22:46:05-06:00
# @Email:  silentcat@protonmail.com
# @Last modified by:   silentcat
# @Last modified time: 2019-11-24T23:02:27-06:00

# Task: Create a random maze generator that creates a maze
# of a particular size according to user input.
# Solution: Think of the maze as being composed of individual cells
# each of which, when connected together, create the grid. Each
# cell will have a dictionary that shows their links to other cells.
# A tostr method will allow us to render the maze from memory.

require 'grid'
require 'binary_tree'

puts "Rows: "
rows = gets
puts "Columns: "
columns = gets
grid = Grid.new(Integer(rows), Integer(columns))
BinaryTree.on(grid)
puts grid
