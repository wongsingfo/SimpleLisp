# SimpleLisp
An simple implement of scheme lisp :)

# Todo

- string type
- garbage collection
- lazy evaluate

# Brief explanation

## Compile 

```
gcc control.c symbol.c number.c exception.c eval.c parser.c atom.c -g
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

