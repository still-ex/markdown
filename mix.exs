defmodule Mix.Tasks.Compile.Hoedown do
  def run(_) do
    {make, args} =
      case :os.type() do
        {:win32, _} -> {"nmake", ["/F", "Makefile.win", "priv\\markdown.dll"]}
        {_, :freebsd} -> {"gmake", ["priv/markdown.so"]}
        {:unix, _} -> {"make", ["priv/markdown.so"]}
      end

    case System.cmd(make, args, stderr_to_stdout: true) do
      {result, 0} ->
        IO.binwrite(result)
        :ok

      {result, _} ->
        IO.binwrite(result)
        :error
    end
  end
end

defmodule Markdown.Mixfile do
  use Mix.Project

  @version "0.1.1"

  def project do
    [
      app: :markdown,
      version: @version,
      elixir: ">= 0.14.3 and < 2.0.0",
      description: "A simple Elixir Markdown to HTML conversion library.",
      package: package(),
      compilers: [:hoedown, :elixir, :app],
      deps: deps()
    ]
  end

  def application do
    []
  end

  defp deps do
    []
  end

  defp package() do
    [
      name: "still_markdown",
      licenses: ["Unlicense"],
      links: %{"GitHub" => "https://github.com/subvisual/markdown"},
      files: [
        "src/**/*.c",
        "src/**/*.h",
        "src/hoedown/Makefile*",
        "src/hoedown/hoedown.def",
        "src/hoedown/html*",
        "priv/.gitignore",
        "lib",
        "*LICENSE*",
        "mix.exs",
        "README*",
        "mix*",
        "Makefile*"
      ]
    ]
  end
end
