# Comonads
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
let id a = a
let compose f g x = f (g x)
```
## Introduction
- Monad - composing kleisli arrows
```OCaml
'a -> 'b m
```
- Comonad
```OCaml
'a w -> 'b
```
- CoKleisli arrow - analog of the fish operator
```ocaml
module type CoKleisli = sig
  type 'a w
  val (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c)
end
```
- CoKleisli - identity arrow - dual of return
```OCaml
val extract : 'a w -> 'a
```
- Comonad type
```ocaml
module type Comonad = sig
  type 'a w
  include Functor with type 'a t := 'a w
  val extract : 'a w -> 'a
  val (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c)
end
```
- Reader monad dissection
```OCaml
'a * 'e -> 'b
```
- With currying
```OCaml
'a -> ('e -> 'b)
```
- They already have the co-Kleisli arrows
```ocaml
type ('e, 'a) product = P of 'e * 'a
```
- Implementing composition for product
```ocaml
let (=>=) f g = fun (P (e, a)) ->
  let b = f (P (e, a)) in
  let c = g (P (e, b)) in
  c
```
- Implementing extract for product
```ocaml
let extract (P (e, a)) = a
```
- Product comonad can be used to perform exactly the same computations as the reader monad.
- Reader functor is the right adjoint of the product functor
### Dissecting the composition
```OCaml
module CoKleisliImpl = struct
type 'a w
let (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c) = fun f g ->
  g ...
end
```
- Dual of bind is extend
```OCaml
val extend : ('a w -> 'b) -> 'a w -> 'b w
```
- Implementing composition using extend
```ocaml
module type CoKleisliExtend = sig
  type 'a w
  val extend : ('a w -> 'b) -> 'a w -> 'b w
end
module CoKleisliImpl(C : CoKleisliExtend) = struct
  type 'a w = 'a C.w
  let (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c) = fun f g ->
    compose g (C.extend f)
end
```
- Duplicate
```OCaml
val duplicate : 'a w -> 'a w w
```
- Three equivalent definitions of co-monad - Co-Kleisli arrows, extends or duplicate
```ocaml
module type ComonadBase = sig
  type 'a w
  include Functor with type 'a t = 'a w
  val extract : 'a w -> 'a
end

module type ComonadDuplicate = sig
  type 'a w
  val duplicate : 'a w -> 'a w w
end

module type ComonadExtend = sig
  type 'a w
  val extend : ('a w -> 'b) -> 'a w -> 'b w
end

module type Comonad = sig
  type 'a w
  include ComonadBase with type 'a w := 'a w
  include ComonadExtend with type 'a w := 'a w
  include ComonadDuplicate with type 'a w := 'a w
end

(* Construct a full comonad instance using one of the following modules *)
module rec ComonadImplViaExtend: functor(C:ComonadBase)(D:ComonadDuplicate with type 'a w = 'a C.w) -> Comonad with type 'a w = 'a C.w = functor(C:ComonadBase)(D:ComonadDuplicate with type 'a w = 'a C.w) -> struct
  include C
  include D
  let extend f wa = (C.fmap f) (D.duplicate wa)
end and ComonadImplViaDuplicate: functor (C:ComonadBase)(E:ComonadExtend with type 'a w = 'a C.w) -> Comonad with type 'a w = 'a C.w = functor(C:ComonadBase)(E:ComonadExtend with type 'a w = 'a C.w) -> struct
  include C
  include E
  let duplicate (wa : 'a w):'a w w = E.extend id wa
end
```
### Stream comonad
```ocaml
type 'a stream = Cons of 'a * 'a stream Lazy.t;;
```
- Functor instance
```ocaml
module StreamFunctor : Functor with type 'a t = 'a stream = struct
  type 'a t = 'a stream
  let rec fmap f = function
    | Cons (x, xs) -> Cons (f x, Lazy.from_val (fmap f (Lazy.force xs)))
end
```
- Get the first element of stream - extract
```ocaml
let extract (Cons (x, _)) = x
```
- Duplicate produces stream of streams
```ocaml
let rec duplicate (Cons (x, xs)) = Cons (Cons (x, xs), Lazy.from_val (duplicate (Lazy.force xs)))
```
- Complete comonad instance
```ocaml
(* Implement Extract *)
module StreamComonadBase(F:Functor with type 'a t = 'a stream):ComonadBase with type 'a w = 'a stream = struct
  type 'a w = 'a stream
  include F
  let extract (Cons (x, _)) = x
end

(* Implement duplicate *)
module StreamComonadDuplicate : ComonadDuplicate with type 'a w = 'a stream = struct
  type 'a w = 'a stream
  let rec duplicate (Cons (x, xs)) = Cons (Cons (x, xs), Lazy.from_val (duplicate (Lazy.force xs)))
end

(* Generate full Comonad Instance *)
module StreamComonad : Comonad with type 'a w = 'a stream = ComonadImplViaExtend(StreamComonadBase(StreamFunctor))(StreamComonadDuplicate)
```
- Analog of advance
```ocaml
let tail (Cons (_, xs)) = Lazy.force xs
```
- sum
```ocaml
let rec sum_s n (Cons (x, xs)) = 
  if n <= 0 then 0 else x + sum_s (n - 1) (Lazy.force xs)
```
- average
```ocaml
let average n stm = Float.(of_int (sum_s n stm) /. of_int n)
```
- movingAverage
```ocaml
let moving_average n = StreamComonad.extend (average n)
```
