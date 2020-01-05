# @Author: S. Sharma <silentcat>
# @Date:   2019-11-24T22:32:54-06:00
# @Email:  silentcat@protonmail.com
# @Last modified by:   silentcat
# @Last modified time: 2019-11-25T18:07:44-06:00

class Cell
  attr_reader :row, :column
  attr_accessor :north, :south, :east, :west

  def initialize(row, column)
    @row, @column = row, column
    @links = {}
  end

  def link(cell, bidi=true)
    @links[cell] = true
    cell.link(self, false) if bidi
    self
  end

  def unlink(cell, bidi=true)
    @links.delete(cell)
    cell.unlink(self, false) if bidi
    self
  end

  def links
    @links.keys
  end

  def linked?(cell)
    @links.key?(cell)
  end

  def neighbors
    list = []
    list << north if north
    list << south if south
    list << east if east
    list << west if west
    list
  end
end
