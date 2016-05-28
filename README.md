# SimpleLisp
An simple implement of scheme lisp :)

# Todo

- string type
- garbage collection
- lazy evaluate

# brief explanation

## Compile 

```
gcc control.c symbol.c number.c exception.c eval.c parser.c atom.c -g
```

## install package

These packages must be installed in order! 
Install `exception` first, then the `symbol` & `number`, then the `atom`, etc..

-- parser
-- eval
  -- atom
    -- symbol
    -- number
      -- exception

