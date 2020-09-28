defmodule MarkdownTest do
  use ExUnit.Case

  doctest Markdown

  describe "to_html/2" do
    test "with autolink: true" do
      markdown = "http://devintorr.es/"

      html = Markdown.to_html(markdown, autolink: true)

      assert html =~ ~r[<a href="http://devintorr.es/">]
    end

    test "with disable_indented_code: true" do
      markdown = """
          1 + 1
      """

      html = Markdown.to_html(markdown, disable_indented_code: true)

      assert html == "<p>1 + 1</p>\n"
    end

    test "with escape: true" do
      markdown = "<p>This is HTML</p>"

      html = Markdown.to_html(markdown, escape: true)

      assert html == "<p>&lt;p&gt;This is HTML&lt;/p&gt;</p>\n"
    end

    test "with fenced_code: true" do
      markdown = """
      ```
      Markdown.to_html(markdown)
      ```
      """

      html = Markdown.to_html(markdown, fenced_code: true)

      assert html =~ ~r/<code>/
    end

    test "with hard_wrap: true" do
      markdown = """
      This has
      line breaks
      """

      html = Markdown.to_html(markdown, hard_wrap: true)

      assert html == "<p>This has<br>\nline breaks</p>\n"
    end

    test "with highlight: true" do
      markdown = "==mark==down"

      html = Markdown.to_html(markdown, highlight: true)

      assert html == "<p><mark>mark</mark>down</p>\n"
    end

    test "with math: true" do
      markdown = "Einstein says $$e = mc^{2}$$"

      html = Markdown.to_html(markdown, math: true)

      assert html == "<p>Einstein says \\(e = mc^{2}\\)</p>\n"
    end

    test "with math_explicit: true" do
      markdown = "Einstein says $$e = mc^{2}$$, not $e = mc$"

      html = Markdown.to_html(markdown, math: true, math_explicit: true)

      assert html == "<p>Einstein says \\[e = mc^{2}\\], not \\(e = mc\\)</p>\n"
    end

    test "with no_intra_emphasis: true" do
      markdown = "This is _very_ im_pre_ssive"

      html = Markdown.to_html(markdown, no_intra_emphasis: true)

      assert html == "<p>This is <em>very</em> im_pre_ssive</p>\n"
    end

    test "with quote: true" do
      markdown = ~s(This will be "quoted")

      html = Markdown.to_html(markdown, quote: true)

      assert html == "<p>This will be <q>quoted</q></p>\n"
    end

    test "with skip_html: true" do
      markdown = "These tags will be <span>stripped</span>"

      html = Markdown.to_html(markdown, skip_html: true)

      assert html == "<p>These tags will be stripped</p>\n"
    end

    test "with space_headers: true" do
      markdown = "#This is not a header"

      html = Markdown.to_html(markdown, space_headers: true)

      assert html == "<p>#This is not a header</p>\n"
    end

    test "with strikethrough: true" do
      markdown = "Strike ~~One~~ ~~Two~~ Three"

      html = Markdown.to_html(markdown, strikethrough: true)

      assert html == "<p>Strike <del>One</del> <del>Two</del> Three</p>\n"
    end

    test "with superscript: true" do
      markdown = "Here ^we^^go"

      html = Markdown.to_html(markdown, superscript: true)

      assert html == "<p>Here <sup>we<sup><sup>go</sup></sup></sup></p>\n"
    end

    test "with tables: true" do
      markdown = """
      |  J  |  O  |
      | --- | --- |
      |  S  |  É  |
      """

      html = Markdown.to_html(markdown, tables: true)

      assert html =~ ~r/<table>/
    end

    test "with underline: true" do
      markdown = "This is not emphasis, this is _underline_"

      html = Markdown.to_html(markdown, underline: true)

      assert html == "<p>This is not emphasis, this is <u>underline</u></p>\n"
    end

    test "with use_xhtml: true" do
      markdown = """
      This is XHTML because of the self closing
      ![img tag](#img-tag)
      """

      html = Markdown.to_html(markdown, use_xhtml: true)

      expected_html = """
      <p>This is XHTML because of the self closing
      <img src="#img-tag" alt="img tag"/></p>
      """

      assert html == expected_html
    end
  end
end
