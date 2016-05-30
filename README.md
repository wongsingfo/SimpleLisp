# SimpleLisp
An simple implement of scheme lisp :)

# Todo

- string type
- garbage collection
- lazy evaluate

# Brief explanation

## Compile 

```
gcc control.c symbol.c number.c exception.c eval.c parser.c atom.c -Wall -O2
```

## install package

These packages must be installed in order! 
Install `exception` first, then the `symbol` & `number`, then the `atom`, etc..

```
-- parser
-- eval
  -- atom
    -- symbol
    -- number
      -- exception
```

# Test

## lambda

```
> (define x 2)
< ()
> ((lambda (f)
           ((lambda (x)     
                    (f 3))
            4))
   (lambda (y) (+ x y)))
< 5
```

The representation of `lambda` and `environment` is shown below.

```
> (define f (lambda (x y) (+ x y)))
< ()
> f
< (lambda_tag (x y) (+ x y) environment_tag (f lambda_tag (...) (...) environment_tag (...) (...) (...) (...) (...) (...)) (if . if) (lambda . lambda) (define . define) (= . =) (+ . +))
```

## cons, car, cdr

```
> (define cons (lambda (x y) 
    (lambda (c)
      (if (= c 0) x y))))
< ()
> (define car (lambda (x)
    (x 0)))
< ()
> (define cdr (lambda (x)
    (x 1)))
< ()
> (define x (cons 100 200))
< ()
> (cdr x)
< 200
> (car x)
< 100
```

## stream

```
> (define stream-car (lambda (stream) (car stream)))
< ()
> (define stream-cdr (lambda (stream) ((cdr stream))))
< ()
> (define ones (cons-stream 1 ones))
< ()
> (car ones)
< 1
> (car (cdr ones))
< lambda_tag
> (stream-car ones)
< 1
> (stream-car (stream-cdr ones))
< 1
> (define add-stream (lambda (s1 s2)
    (cons-stream (+ (stream-car s1)
                    (stream-car s2))
                 (add-stream (stream-cdr s1)
                             (stream-cdr s2)))))
< ()
> (define integers (cons-stream 1 (add-stream ones integers)))
< ()
> (stream-car integers)
< 1
> (stream-car (stream-cdr integers))
< 2
> (stream-car (stream-cdr (stream-cdr (stream-cdr integers)))) 
< 4
> (stream-cdr (stream-cdr (stream-cdr integers)))
< (4 lambda_tag () (add-stream (...) (...)) environment_tag (s2 3 lambda_tag () (...) ...) (s1 1 lambda_tag () ones ...) (integers 1 lambda_tag () (...) ...) (add-stream lambda_tag (...) (...) ...) (ones 1 lambda_tag () ones ...) ...)
```
