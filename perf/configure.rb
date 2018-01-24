require 'open-uri'
require 'fileutils'
require 'zip'

WD = 'temp/'
CACHE = WD + 'cache'
ZIPS = [
  ['https://github.com/iboB/picobench/archive/v1.03.zip', 'pb.zip'],
  ['https://github.com/gabime/spdlog/archive/v0.16.3.zip', 'spd.zip'],
]

class CacheEntry
  def initialize(url, zip)
    @url = url
    @zip = zip
    @files = []
    @check = false
  end

  def <<(fname)
    @files << fname
  end

  def check!
    @check = true
  end

  def check?
    @check
  end

  attr_reader :url, :zip
  attr_accessor :files
end

# read cache
cache = {}
if File.file?(CACHE)
  puts 'Found cache'
  latest = nil # latest entry in cache
  File.open(CACHE, 'r').each_line do |l|
    l.rstrip!
    next if l.empty?
    file = l.lstrip
    if file != l # indented line
      raise "Bad cache! @'#{l}'" unless latest
      latest << file
    else
      url, zip = *l.split(/ /)
      latest = CacheEntry.new(url, zip)
      cache[url] = latest
    end
  end
end


def wget(url, target)
  FileUtils.mkdir_p(File.dirname(target))
  File.open(target, 'wb') do |f|
    IO.copy_stream(open(url, 'rb'), f)
  end
end

# returns extracted files
def unzip(zip, dest_dir)
  files = []
  Zip::File.open(zip) do |z|
    z.each do |f|
      path = File.join(dest_dir, f.name)
      FileUtils.mkdir_p(File.dirname(path))
      # symlinks are admin-only on default windows settings
      # so far there is no use-case for which we would care about them
      # so... ignore them for now
      z.extract(f, path) if !z.get_entry(f).symlink?
      files << path
    end
  end
  files
end

ZIPS.each do |z|
  l = cache[z[0]]
  if l
    puts "Found #{z[0]} in cache"
    l.check!
    next
  end
end

# remove all non-checked entries
cache.each do |url, e|
  next if e.check?
  print "Removing files for #{url}..."
  File.delete(WD + e.zip)
  e.files.each do |f|
    if File.file?(f)
      File.delete(f)
    elsif File.directory?(f)
      FileUtils.rm_rf(f)
    end
  end
  puts " done."
end

# download
ZIPS.each do |z|
  url, zip = *z
  next if cache[url]

  # we don't have this zip
  begin
    e = CacheEntry.new(url, zip)
    cache[url] = e

    print "Downloading #{url}..."
    wget(url, WD + zip)
    puts " done."

    print "Extracting #{zip}..."
    e.files = unzip(WD + zip, WD)
    puts " done."

    e.check!
  rescue Exception => e
    puts
    puts e.message
    puts e.backtrace
  end
end

# write cache
File.open(CACHE, "w") do |f|
  cache.each do |url, e|
    next unless e.check?
    f.puts("#{url} #{e.zip}")
    e.files.each do |file|
      f.puts("    #{file}")
    end
  end
end
