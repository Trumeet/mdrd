# mdrd

Simple CGI Markdown renderer.

## Building

Requirements:

* cmark

* fcgi

* cmake

* pkgconfig

```shell
$ mkdir cmake-build-release/
$ cd cmake-build-release/
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j mdrd
```

## Running

Start a FCGI parent process:

```shell
$ spawn-fcgi -p 8080 -n ./mdrd
```

Setup your web server with document root, fastcgi parameters and fastcgi port. Nginx example:

```
server {
        root        /path/to/markdown/root;
        autoindex   on;
        index       index.html index.md;
	location    ~ \.md$ {
        	include             fastcgi_params;
		fastcgi_pass        127.0.0.1:8080;
	}
}
```

You are ready to go.

## License

WTFPL. Experimental project. No warranty.
