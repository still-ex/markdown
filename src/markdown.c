#include <string.h>
#include <stdint.h>
#include <time.h>

#include "erl_nif.h"

#include "buffer.h"
#include "document.h"
#include "html.h"

#define OUTPUT_UNIT 128
#define ARBITRARY_DIRTY_NIF_THRESHOLD 50000

typedef struct {
  ERL_NIF_TERM atom_autolink;
  ERL_NIF_TERM atom_disable_indented_code;
  ERL_NIF_TERM atom_escape;
  ERL_NIF_TERM atom_fenced_code;
  ERL_NIF_TERM atom_hard_wrap;
  ERL_NIF_TERM atom_highlight;
  ERL_NIF_TERM atom_math;
  ERL_NIF_TERM atom_math_explicit;
  ERL_NIF_TERM atom_no_intra_emphasis;
  ERL_NIF_TERM atom_quote;
  ERL_NIF_TERM atom_skip_html;
  ERL_NIF_TERM atom_space_headers;
  ERL_NIF_TERM atom_strikethrough;
  ERL_NIF_TERM atom_superscript;
  ERL_NIF_TERM atom_tables;
  ERL_NIF_TERM atom_true;
  ERL_NIF_TERM atom_underline;
  ERL_NIF_TERM atom_use_xhtml;
} markdown_priv;

int
apply_extension(const ERL_NIF_TERM* tuple, ERL_NIF_TERM field, ERL_NIF_TERM atom_true, unsigned int flag, unsigned int* extensions) {
    if (enif_compare(tuple[0], field) == 0) {
      if (enif_compare(tuple[1], atom_true) == 0) {
        *extensions |= flag;
        return 1;
      }
    }

    return 0;
}

int
apply_markdown_extensions(const ERL_NIF_TERM* tuple, markdown_priv* priv, unsigned int* extensions) {
    if (apply_extension(tuple, priv->atom_autolink, priv->atom_true, HOEDOWN_EXT_AUTOLINK, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_disable_indented_code, priv->atom_true, HOEDOWN_EXT_DISABLE_INDENTED_CODE, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_fenced_code, priv->atom_true, HOEDOWN_EXT_FENCED_CODE, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_highlight, priv->atom_true, HOEDOWN_EXT_HIGHLIGHT, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_math, priv->atom_true, HOEDOWN_EXT_MATH, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_math_explicit, priv->atom_true, HOEDOWN_EXT_MATH_EXPLICIT, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_no_intra_emphasis, priv->atom_true, HOEDOWN_EXT_NO_INTRA_EMPHASIS, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_quote, priv->atom_true, HOEDOWN_EXT_QUOTE, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_space_headers, priv->atom_true, HOEDOWN_EXT_SPACE_HEADERS, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_strikethrough, priv->atom_true, HOEDOWN_EXT_STRIKETHROUGH, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_superscript, priv->atom_true, HOEDOWN_EXT_SUPERSCRIPT, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_tables, priv->atom_true, HOEDOWN_EXT_TABLES, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_underline, priv->atom_true, HOEDOWN_EXT_UNDERLINE, extensions))
        return 1;

    return 0;
}

int
apply_html_extensions(const ERL_NIF_TERM* tuple, markdown_priv* priv, unsigned int* extensions) {
    if (apply_extension(tuple, priv->atom_escape, priv->atom_true, HOEDOWN_HTML_ESCAPE, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_hard_wrap, priv->atom_true, HOEDOWN_HTML_HARD_WRAP, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_skip_html, priv->atom_true, HOEDOWN_HTML_SKIP_HTML, extensions))
        return 1;

    if (apply_extension(tuple, priv->atom_use_xhtml, priv->atom_true, HOEDOWN_HTML_USE_XHTML, extensions))
        return 1;

    return 0;
}

static ERL_NIF_TERM
do_to_html(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary input;
  ErlNifBinary output;

  markdown_priv* priv;
  ERL_NIF_TERM options;
  ERL_NIF_TERM term;
  const ERL_NIF_TERM* tuple;
  int tuple_size;
  unsigned int markdown_extensions;
  unsigned int html_extensions;

  hoedown_buffer* ob;
  hoedown_document* document;
  hoedown_renderer* renderer;

#ifdef PROFILE
  clock_t prof_start_time, prof_end_time;
  double elapsed_time;
#endif

  if (enif_inspect_binary(env, argv[0], &input) == 0) {
    return enif_make_badarg(env);
  }

  if (input.size < 1) {
    return argv[0];
  }

  priv = enif_priv_data(env);
  options = argv[1];
  markdown_extensions = 0;
  html_extensions = 0;

  while (enif_get_list_cell(env, options, &term, &options) != 0) {
    if (enif_get_tuple(env, term, &tuple_size, &tuple) != 0) {
      if (tuple_size > 1) {
          if(apply_markdown_extensions(tuple, priv, &markdown_extensions))
              continue;

          if(apply_html_extensions(tuple, priv, &html_extensions))
              continue;
      }
    }
  }

  ob = hoedown_buffer_new(OUTPUT_UNIT);
  renderer = hoedown_html_renderer_new(html_extensions, 0);
  document = hoedown_document_new(renderer, markdown_extensions, 16);

#ifdef PROFILE
  prof_start_time = clock();
#endif

  hoedown_document_render(document, ob, (uint8_t*) input.data, input.size);

#ifdef PROFILE
  prof_end_time = clock();

  if(prof_start_time == (clock_t)-1 || prof_end_time == (clock_t)-1) {
      fprintf(stderr, "[markdown.c]: Error profiling\n");
  }

  fprintf(stderr, "[markdown.c]: Input size: %lu\n", input.size);

  elapsed_time = (double)(prof_end_time - prof_start_time) / CLOCKS_PER_SEC;

  if (elapsed_time < 1)
      /* time in millis */
      fprintf(stderr, "[markdown.c]: Parsing time: %.3fms.\n", elapsed_time * 1000);
  else
      /* time in seconds */
      fprintf(stderr, "[markdown.c]: Parsing time: %.5fs.\n", elapsed_time);
#endif

  enif_release_binary(&input);
  hoedown_html_renderer_free(renderer);
  hoedown_document_free(document);

  enif_alloc_binary(ob->size, &output);
  memcpy(output.data, ob->data, ob->size);
  hoedown_buffer_free(ob);

  return enif_make_binary(env, &output);
}

static ERL_NIF_TERM
to_html(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ErlNifBinary input;

  if (enif_inspect_binary(env, argv[0], &input) == 0) {
    return enif_make_badarg(env);
  }

  if (input.size < 1) {
    return argv[0];
  }

  if (input.size > ARBITRARY_DIRTY_NIF_THRESHOLD) {
#ifdef PROFILE
      fprintf(stderr, "[markdown.c]: Input above arbitrary threshold. Running dirty NIF.\n");
#endif
      enif_schedule_nif(env, "do_to_html", ERL_NIF_DIRTY_JOB_CPU_BOUND, do_to_html, argc, argv);
  } else {
#ifdef PROFILE
      fprintf(stderr, "[markdown.c]: Input below arbitrary threshold. Running regular NIF.\n");
#endif
      do_to_html(env, argc, argv);
  }
}

static ErlNifFunc funcs[] = {
  { "to_html", 2, to_html }
};

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  markdown_priv* data = enif_alloc(sizeof(markdown_priv));
  if (data == NULL) {
    return 1;
  }

  data->atom_autolink = enif_make_atom(env, "autolink");
  data->atom_disable_indented_code = enif_make_atom(env, "disable_indented_code");
  data->atom_escape = enif_make_atom(env, "escape");
  data->atom_fenced_code = enif_make_atom(env, "fenced_code");
  data->atom_hard_wrap = enif_make_atom(env, "hard_wrap");
  data->atom_highlight = enif_make_atom(env, "highlight");
  data->atom_math = enif_make_atom(env, "math");
  data->atom_math_explicit = enif_make_atom(env, "math_explicit");
  data->atom_no_intra_emphasis = enif_make_atom(env, "no_intra_emphasis");
  data->atom_quote = enif_make_atom(env, "quote");
  data->atom_skip_html = enif_make_atom(env, "skip_html");
  data->atom_space_headers = enif_make_atom(env, "space_headers");
  data->atom_strikethrough = enif_make_atom(env, "strikethrough");
  data->atom_superscript = enif_make_atom(env, "superscript");
  data->atom_tables = enif_make_atom(env, "tables");
  data->atom_true = enif_make_atom(env, "true");
  data->atom_underline = enif_make_atom(env, "underline");
  data->atom_use_xhtml = enif_make_atom(env, "use_xhtml");

  *priv = (void*) data;
  return 0;
}

static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info) {
  return load(env, priv, info);
}

static void
unload(ErlNifEnv* env, void* priv) {
  enif_free(priv);
}

ERL_NIF_INIT(Elixir.Markdown, funcs, &load, &reload, &upgrade, &unload)
