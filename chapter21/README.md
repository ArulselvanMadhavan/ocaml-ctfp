# Monads and Effects
## Utilities used by code below
```ocaml
module type Monad_Join =  sig
  type 'a m
  val join : 'a m m -> 'a m
  val return : 'a -> 'a m
end
module type Monad_Bind =
  sig
    type 'a m
    val ( >>= ) : 'a m -> ('a -> 'b m) -> 'b m
    val return : 'a -> 'a m
  end
module type Monoid = sig
  type a
  val mempty : a
  val mappend : a -> a -> a
end  
```
## Introduction
- Partiality - may not terminate
- Nondeterminism - return many results
- Side effects - access/modify state
- Exceptions - Partial functions
- Continuations - ability to save state of the program and then restore it
- Interactive input
- Interactive output
## Partiality
```ocaml
module List_Monad : Monad_Join = struct
  type 'a m = 'a list
  let join = List.concat
  let return a = [a]
end
```
- Bind using join and fmap
```ocaml
let ( >>= ) xs k = List.concat (List.map k xs)
```
- Triples in OCaml
```ocaml
module Pythagorean = struct

  let (let*) = Fn.flip Gen.flat_map

  let (let+) x f = Gen.map f x

  let guard b = if b then Gen.return () else Gen.empty

  let triples = 
    let* z = Gen.init (fun i -> i + 1) in
    let* x = Gen.init ~limit:z (fun i -> i + 1) in
    let* y = Gen.init ~limit:z (fun i -> i + x) in
    let+ _ = guard (x * x + y * y = z * z) in
    Gen.return (x, y, z)
end
```
- guard for List
```ocaml
let guard = function
  true -> [()]
  | false -> []
```
- Triples alternate
```ocaml
module Pythagorean = struct

  let (let*) = Fn.flip Gen.flat_map

  let (let+) x f = Gen.map f x

  let guard b = if b then Gen.return () else Gen.empty

  let triples = 
    let* z = Gen.init (fun i -> i + 1) in
    let* x = Gen.init ~limit:z (fun i -> i + 1) in
    let* y = Gen.init ~limit:z (fun i -> i + x) in
    if (x * x + y * y = z * z) then
    Gen.return (x, y, z) else Gen.empty
end
```
### Reader
- Reader type
```ocaml
type ('e, 'a) reader = Reader of ('e -> 'a)
```
- run_reader
```ocaml
let run_reader (Reader f) e = f e
```
- Bind implementation for reader
```OCaml
let (>>=) ra k = Reader (fun e -> ...)
```
```OCaml
let (>>=) ra k = Reader (fun e -> 
  let a = run_reader ra e in
  ...)
```
```OCaml
let (>>=) ra k = Reader (fun e ->
  let a = run_reader ra e in
  let rb = k a in
  ...)
```
```ocaml
let (>>=) ra k = Reader (fun e ->
  let a = run_reader ra e in
  let rb = k a in
  run_reader rb e)
```
```ocaml
module ReaderMonad(T : sig type t end) : Monad_Bind = struct
  type 'a m = (T.t, 'a) reader
  let return a = Reader (fun e -> a)
  let (>>=) ra k = Reader (fun e -> run_reader (k (run_reader ra e)) e)
end
```
### Write Only State
```ocaml
type ('w, 'a) writer = Writer of ('a * 'w);;
```
```ocaml
let run_writer (Writer (a, w)) = (a, w)
```
- Writer Instance
```ocaml
module WriterMonad(W : Monoid):Monad_Bind = struct
  type 'a m = (W.t, 'a) writer
  
  let return a = Writer (a, W.mempty)
  
  let (>>=) (Writer (a, w)) k = 
    let (a', w') = run_writer (k a) in
    Writer (a', W.mappend w w')
end
```
### State
- Combines Reader and Writer
```ocaml
type ('s, 'a) state = State of ('s -> ('a * 's))
```
- runstate
```ocaml
let run_state (State f) s = f s;;
```
- bind
```ocaml
let (>>=) sa k = State (fun s -> 
  let (a, s') = run_state sa s in
  let sb = k a in
  run_state sb s')
```
- Monad Instance
```ocaml
module State_Monad(S : sig type t end) : Monad_Bind = struct
  
  type 'a m = (S.t, 'a) state

  let (>>=) sa k = State (fun s -> 
    let (a, s') = run_state sa s in
    let sb = k a in
    run_state sb s')

  let return a = State (fun s -> (a, s))
end
```
### Exceptions
- Partial functions(throws exceptions) can be converted to total functions
- Ex: Maybe, Either
```ocaml
module OptionMonad : Monad_Bind = struct
  type 'a m = 'a option

  let (>>=) = function
    | Some a -> fun k -> k a
    | None   -> fun _ -> None
 
  let return a = Some a
end
```
### Continuations
