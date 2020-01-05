# @Author: S. Sharma <silentcat>
# @Date:   2019-11-24T22:43:47-06:00
# @Email:  silentcat@protonmail.com
# @Last modified by:   silentcat
# @Last modified time: 2019-12-18T20:52:38-06:00

class BinaryTree

  def self.on(grid, rstart=0, rend=grid.rows-1, cstart=0, cend=grid.columns-1)
    for row in rstart..rend
      for column in cstart..cend
        cell = grid[row, column]
        neighbors = []
        neighbors << cell.north if cell.north
        neighbors << cell.east if cell.east

        index = rand(neighbors.length)
        neighbor = neighbors[index]

        cell.link(neighbor) if neighbor
      end
    end

    grid
  end
end
