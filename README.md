# Markdown

A simple Elixir [Markdown][1] to HTML conversion library.

Implemented entirely as a [NIF][2] binding to the [Hoedown][3] library.

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

- [ ] Use dirty schedulers [when available][4]
- [ ] ANSI renderer

[1]: http://daringfireball.net/projects/markdown/
[2]: http://www.erlang.org/doc/tutorial/nif.html
[3]: https://github.com/hoedown/hoedown
[4]: https://github.com/zambal/markdown/commit/792a414de074d96ac47b5b52d212b7197ce04462
[5]: https://github.com/hoedown/hoedown/blob/8ab482af918f6b5c296dca80f0c67e924c06b5c2/bin/hoedown.c#L49
