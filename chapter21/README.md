# Monads and Effects
## Utilities used by code below
```ocaml
module type Monad_Join =  sig
  type 'a m
  val join : 'a m m -> 'a m
  val return : 'a -> 'a m
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
