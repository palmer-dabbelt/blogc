# blogc

[![Build Status](https://semaphoreci.com/api/v1/projects/bd67545c-8593-4a37-ba94-ef1187a6d58d/402577/badge.svg)](https://semaphoreci.com/rafaelmartins/blogc)      

A blog compiler.


## Design Notes

The main idea is simple: a source file is read by the compiler, and a result file is written by the compiler.

The source file must provide (almost) all the data needed to build the result file, including any variables. The result file is built using a template, that defines how the information provided by the source file should be used to generate a reasonable result file.

The compiler will always generate a single file, no matter how many source files are provided. If more than one source file is provided, the compiler (and the template) must know how to convert them to a single result file.

The templates can define blocks. These are the block rules:

- If something is defined outside of blocks, its raw content should be always included.
- If something is defined inside a block, it should only be included if the block matches the compiler argument expectations, e.g.:
    - ``entry`` should be include if just one source file is provided.
    - ``listing`` should be included if more than one source file is provided, being included once for each source file, if the compiler is called with ``-l``.
    - ``listing_once`` should be inclueded if more than one source file is provided, but only once, if the compiler is called with ``-l``.
- Template blocks can be defined multiple times in the same template, but can't be nested.

The variables defined in the source file are only available inside of blocks. If something does not depends on the source files, and is global, it must be hardcoded in the template, for the sake of simplicity.

The templates can use conditional statements: ``{% if VARIABLE %}`` or ``{% if not VARIABLE %}``, and ``{% endif %}``. They check if a variable is defined or not. As variables are not available outside of blocks, these conditional statements can't be defined outside of blocks as well.

Variables are not available in ``listing_once`` blocks, because it is not possible to guess which source file would provide the variable contents.

As the compiler is output-agnostic, Atom feeds and sitemaps should be generated using templates as well.

The compiler won't touch the content. If the user write pre-formatted text it will stay pre-formatted, and the user may want to enclose the ``{{ CONTENT }}`` statement with ``<pre>`` and ``</pre>`` tags in the templates. For more flexibility, the user can even write raw HTML in the source content.

The compiler is designed to be easily used with any POSIX-compatible implementation of ``make``.


### Source file syntax

```
TITLE: My nice post
DATE: 2007-04-05 12:30:00
----
test content.

more test content.
```

If more than one source file is provided to the compiler with the ``-t`` argument, they won't be sorted, and will be included by ``listing`` blocks in the order that they were provided in the command line.

Variables are single-line, and all the whitespace characters, including tabs, before the leading non-whitespace character and after the trailing non-whitespace character will be removed.

Raw content is available in template blocks as the ``CONTENT`` variable.


### Template file syntax

```html
<html>
    <head>
        {% block entry %}
        <title>My cool blog >> {{ TITLE }}</title>
        {% endblock %}
        {% block listing_once %}
        <title>My cool blog - Main page</title>
        {% endblock %}
    </head>
    <body>
        <h1>My cool blog</h1>
        {% block entry %}
        <h2>{{ TITLE }}</h2>
        {% if DATE %}<h4>Published in: {{ DATE }}</h4>{% endif %}
        <pre>{{ CONTENT }}</pre>
        {% endblock %}
        {% block listing_once %}<ul>{% endblock %}
        {% block listing %}<p><a href="{{ FILENAME }}.html">{{ TITLE }}</a>{% if DATE %} - {{ DATE }}{% endif %}</p>{% endblock %}
        {% block listing_once %}</ul>{% endblock %}
    </body>
</html>
```

This template would generate a list of posts, if multiple source files were provided with ``-t``, and single pages for each post, if only one source file was provided. The ``FILENAME`` variable is generated by the compiler, and is the source file base name without its last extension.


## Roadmap

The first release will happen as soon as ``blogc`` can build its own website (http://blogc.org/). ;-)
