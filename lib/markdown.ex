defmodule Markdown do
  @moduledoc """
  Markdown to HTML conversion.

  ## Dirty Scheduling

  This relies on a NIF wrapping the hoedown library.

  By default the NIF is deemed as clean for input lower than 30k characters. For
  inputs over this value, it is likely the render time will take over 1ms and thus
  it should be scheduled on a dirty scheduler.

  Since it is impossible to know beforehand, if an input will take over 1ms to be
  processed, the 30k threshold is considered an arbitrary value. See
  [subvisual/markdown#1](https://github.com/subvisual/markdown/pulls/1).

  This value can be configured by setting the following in your `config/config.exs`:

  ```elixir
  config :markdown, dirty_scheduling_threshold: 50_000
  ```
  """

  @on_load {:init, 0}

  app = Mix.Project.config()[:app]

  def init do
    path = :filename.join(:code.priv_dir(unquote(app)), 'markdown')
    :ok = :erlang.load_nif(path, 0)

    case Application.get_env(:markdown, :dirty_scheduling_threshold) do
      nil -> :ok
      value -> set_nif_threshold(value)
    end

    :ok
  end

  @doc ~S"""
  Converts a Markdown document to HTML:

      iex> Markdown.to_html "# Hello World"
      "<h1>Hello World</h1>\n"
      iex> Markdown.to_html "http://elixir-lang.org/", autolink: true
      "<p><a href=\"http://elixir-lang.org/\">http://elixir-lang.org/</a></p>\n"

  Available output options:

  * `:tables` - Enables Markdown Extra style tables (default: `false`)
  * `:fenced_code` - Enables fenced code blocks (default: `false`)
  * `:autolink` - Automatically turn URLs into links (default: `false`)

  """
  @spec to_html(doc :: String.t()) :: String.t()
  @spec to_html(doc :: String.t(), options :: Keyword.t()) :: String.t()
  def to_html(doc, options \\ [])

  def to_html(_, _) do
    exit(:nif_library_not_loaded)
  end

  def set_nif_threshold(_) do
    exit(:nif_library_not_loaded)
  end
end
