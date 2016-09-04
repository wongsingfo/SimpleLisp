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
< (lambda_tag (x y) (+ x y) ...env...)
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
(define-syntax cons-stream
  (syntax-rules ()
    ((cons-stream a b)
     (cons a (lambda () b))))) 
```

`cons-tream` is a primitive procedure, so the code above is not needed.

```
> (define add-streams
  (lambda (x y)
    (cons-stream
      (+ (stream-car x) (stream-car y))
      (add-streams (stream-cdr x) (stream-cdr y)))))
< ()
> (define stream-car (lambda (stream) (car stream)))
< ()
> (define stream-cdr (lambda (stream) ((cdr stream))))
< ()
> (define fibs (cons-stream 0 (cons-stream 1 (add-streams (stream-cdr fibs) fibs))))
< ()
> (define stream-iter 
  (lambda (stream i)
    (if (= i 0) 
        (stream-car stream)
        (stream-iter (stream-cdr stream) (+ i -1)))))
< ()
> (stream-iter fibs 22)
< 17711
```