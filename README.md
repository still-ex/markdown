# Markdown - Subvisual fork

A simple Elixir [Markdown][markdown] to HTML conversion library.

Implemented entirely as a [NIF][nif] binding to the [Hoedown][hoedown] library.

This fork of [`devinius/markdown`][upstream] was created to keep a stable source
for the `markdown` Elixir package supporting the full range of `hoedown`
configurations options and the use of dirty schedulers. You should be able to
reference any `master` commits on your `mix.exs` dependency declaration, as
they will not disappear.

The base version of this fork is [`d065dbc`][base-commit].

## Installation

Add `markdown` to your list of dependencies on `mix.exs`. Make sure you
reference this repo:

```elixir
def deps do
  # consider adding a ref for a specific commit
  [{:markdown, github: "subvisual/markdown"}]
end
```

## Usage

```iex
iex> Markdown.to_html "# Hello World"
"<h1>Hello World</h1>\n"

iex> Markdown.to_html "http://elixir-lang.org/", autolink: true
"<p><a href=\"http://elixir-lang.org/\">http://elixir-lang.org/</a></p>\n"
```

### Options

All of these default to `false`:

- `:autolink` - Automatically turn URLs into links.
- `:disable_indented_code` - Don't indented code blocks as `<code>`.
- `:escape` - Escape all HTML tags.
- `:fenced_code` - Enables fenced code blocks.
- `:hard_wrap` - Replace line breaks with `<hr>` tags.
- `:highlight` - Replace `==highlight==` blocks with `<mark>` tags.
- `:math` - Parse TeX-based `$$math$$` syntax.
- `:math_explicit` - Requires `math: true`. Parse `$math$` as inline and
  `$$math$$` as blocks, instead of attempting to guess.
- `:no_intra_emphasis` - Don't parse `underscores_between_words` as `<em>` tags.
- `:quote` - Render "quotation marks" as `<q>` tags.
- `:skip_html` - Strip HTML tags.
- `:space_headers` - Require a space after `#` in the headers.
- `:strikethrough` - Parse `~~text~~` as `<del>` tags.
- `:superscript` - Parse `^text` as `<sup>` tags.
- `:tables` - Enables Markdown Extra style tables.
- `:underline` - Parse `_text_` as `<u>` tags.
- `:use_xhtml` - Use XHTML instead of HTML.

You can find examples in the [test module](./test/markdown_test.exs).

#### Dirty Scheduling

By default the NIF is deemed as clean for input lower than 30k characters. For
inputs over this value, it is likely the render time will take over 1ms and thus
it should be scheduled on a dirty scheduler.

Since it is impossible to know beforehand, if an input will take over 1ms to be
processed, the 30k threshold is considered an arbitrary value. See
[subvisual/markdown#1][subvisual/markdown#1].

This value can be configured by setting the following in your `config/config.exs`:

```elixir
config :markdown, dirty_scheduling_threshold: 50_000
```

## Contributing

Feel free to contribute.

If you found a bug, open an issue. You can also open a PR for bugs or new
features. Your PRs will be reviewed and subject to our style guide and linters.

All contributions **must** follow the [Code of Conduct][coc] and
[Subvisual's guides][subvisual-guides].

### TODOs

A list of tasks anyone is free to tackle:

- [ ] ANSI renderer

## About

`markdown` was created by [Devin Torres (@devinus)][devinus] and this fork is
maintained with :heart: by [Subvisual][subvisual].

![Subvisual](https://raw.githubusercontent.com/subvisual/guides/master/github/templates/logos/blue.png)

[markdown]: http://daringfireball.net/projects/markdown/
[nif]: http://www.erlang.org/doc/tutorial/nif.html
[hoedown]: https://github.com/hoedown/hoedown
[upstream]: https://github.com/devinus/markdown
[devinus]: https://github.com/devinus
[base-commit]: https://github.com/devinus/markdown/commit/d065dbcc4e242a85ca2516fdadd0082712871fd8
[coc]: https://github.com/subvisual/markdown/blob/master/CODE_OF_CONDUCT.md
[subvisual-guides]: https://github.com/subvisual/guides
[subvisual]: https://subvisual.com
[subvisual/markdown#1]: https://github.com/subvisual/markdown/pulls/1
