# Monads and Effects
## Utilities used by code below
```ocaml
let flip f x y = f y x
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
  type t
  val mempty : t
  val mappend : t -> t -> t
end
type 'a io = IO of (unit -> 'a)
let put_str s = IO (fun () -> print_string s)
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
let ( >>= ) xs k = List_Monad.join (List.map k xs)
```
- Triples in OCaml
```ocaml
(* This requires the "gen" library,
   after having installed them, execute
   #require "gen";; *)

module Pythagorean = struct

  let (let*) = flip Gen.flat_map

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
  | true -> [()]
  | false -> []
```
- Triples alternate
```ocaml
(* This requires the "gen" library,
   after having installed them, execute
   #require "gen";; *)

module Pythagorean = struct

  let (let*) = flip Gen.flat_map

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
let run_state (State f) s = f s
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
```ocaml
let get = State (fun s -> (s, s))
```
```ocaml
let put s' = State (fun s -> ((), s'))
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
- Continuation type
```ocaml
type ('r, 'a) cont = Cont of (('a -> 'r) -> 'r);;
```
- runCont
```ocaml
let run_cont (Cont k) h = k h
```
- bind for Cont Monad
```OCaml
val (>>=) : (('a -> 'r) -> 'r) -> ('a -> (('b -> 'r) -> 'r)) -> (('b -> 'r) -> 'r)
```
- Step1
```OCaml
let (>>=) ka kab = Cont (fun hb -> ...)
```
- Step2
```OCaml
run_cont ka (fun a -> ...)
```
- Step3
```OCaml
run_cont ka (fun a ->
  let kb = kab a in
  run_cont kb hb)
```
- Monad Instance
```ocaml
module Cont_Monad(R:sig type t end) : Monad_Bind = struct
  type 'a m = (R.t, 'a) cont

  let return a = Cont (fun ha -> ha a)

  let (>>=) ka kab = Cont (fun hb ->
    run_cont ka (fun a ->
    run_cont (kab a) hb))
end
```
### Interactive Input
```ocaml
module type Terminal_IO = sig
  (* OCaml doesn't have a built-in IO type*)
  type 'a io = IO of (unit -> 'a)
  
  val get_char : unit -> char io
end
```
- main 
```OCaml
val main : unit io
```
- As Kleisli arrow
```OCaml
val main : unit -> unit io
```
- IO as a State monad with RealWorld type
```OCaml
type 'a io = realworld -> ('a * realworld)
```
```OCaml
type 'a io = realworld state
```
### Interactive output
```OCaml
val put_str : string -> unit io
```
```OCaml
val put_str : string -> unit
```
- Main function of type unit io 
```ocaml
(* Monad implementation for type io *)
module IOMonad:Monad_Bind with type 'a m = 'a io = struct
    type 'a m = 'a io
    let return x = IO (fun () -> x)
    let (>>=) m f = IO (fun () -> 
        let (IO m') = m in 
        let (IO m'') = f (m' ()) in 
        m'' ()
    )
end

(* main *)
module IO_Main = struct

  let (let*) = IOMonad.(>>=)
  
  let main = 
    let* _ = put_str "Hello" in
    put_str "world!"
end
```
