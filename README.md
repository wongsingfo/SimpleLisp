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
< nil
> ((lambda (f)
           ((lambda (x)     
                    (f 3))
            4))
   (lambda (y) (+ x y)))
< 5
```

## cons, car, cdr

```
> (define cons (lambda (x y) 
    (lambda (c)
      (if (= c 0) x y))))
< nil
> (define car (lambda (x)
    (x 0)))
< nil
> (define cdr (lambda (x)
    (x 1)))
< nil
> (define x (cons 100 200))
< nil
> (cdr x)
< 200
> (car x)
< 100
```

