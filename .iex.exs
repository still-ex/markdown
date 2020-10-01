defmodule Markdown.Fxt do
  def xsmall, do: File.read!("test/fixtures/xsmall.md")

  def small, do: File.read!("test/fixtures/small.md")

  def medium, do: File.read!("test/fixtures/medium.md")

  def long, do: File.read!("test/fixtures/long.md")

  def xlong, do: File.read!("test/fixtures/xlong.md")
end

alias Markdown.Fxt
