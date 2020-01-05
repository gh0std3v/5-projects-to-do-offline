# @Author: S. Sharma <silentcat>
# @Date:   2019-11-24T22:46:05-06:00
# @Email:  silentcat@protonmail.com
# @Last modified by:   silentcat
# @Last modified time: 2019-11-24T23:02:27-06:00

require 'grid'
require 'binary_tree'

puts "Rows: "
rows = gets
puts "Columns: "
columns = gets
grid = Grid.new(Integer(rows), Integer(columns))
BinaryTree.on(grid)
puts grid
