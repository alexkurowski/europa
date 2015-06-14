#!/usr/bin/env ruby

require 'rmagick'
include Magick

filename = ARGV[0]

char = {
  w: 16,
  h: 16
}

chars = Array.new(char[:h]) { Array.new(char[:w]) { [] } }

img = Image.read(filename)[0]

def bit p
  return 0 if p.red.to_i == 0
  return 1 if p.red.to_i != 0
end

img.each_pixel do |pixel, x, y|
  i = (x / 8).floor
  j = (y / 8).floor

  chars[i][j] << bit(pixel)
end

File.open('output', 'w') do |f|
  f.truncate 0
  c = 0
  bit_to_write = ''
  for i in 0..15 do
    for j in 0..15 do
      chars[j][i].each do |bit|
        c += 1
        bit_to_write << bit.to_s
        next unless c == 8

        c = 0
        f.write('0x')
        print('0x')
        f.write(bit_to_write.to_i(2).to_s(16).rjust(2, '0'))
        print(bit_to_write.to_i(2).to_s(16).rjust(2, '0'))
        f.write(',')
        print(',')
        bit_to_write = ''

      end
    end
  end
end
