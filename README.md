# Markdown - Subvisual fork

A simple Elixir [Markdown][markdown] to HTML conversion library.

Implemented entirely as a [NIF][nif] binding to the [Hoedown][hoedown] library.

This fork of [`devinius/markdown`][upstream] was created to keep a trustworthy and
stable source for the `markdown` Elixir package with the added configurations
options and a patch for the use of dirty schedulers.

The base version of this fork is [`d065dbc`][base-commit].

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

## TODO

- [ ] ANSI renderer

[markdown]: http://daringfireball.net/projects/markdown/
[nif]: http://www.erlang.org/doc/tutorial/nif.html
[hoedown]: https://github.com/hoedown/hoedown
[upstream]: https://github.com/devinus/markdown
[base-commit]: https://github.com/devinus/markdown/commit/d065dbcc4e242a85ca2516fdadd0082712871fd8
