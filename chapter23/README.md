# Comonads
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
let id a = a
```
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
  val extract : ('a w -> 'b) -> 'a w -> 'b w
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

(* Recursive module definition *)
module ComonadImpl(W : sig type 'a w end)(C : ComonadBase with type 'a w = 'a W.w)(E : ComonadExtend with type 'a w = 'a W.w)(D : ComonadDuplicate with type 'a w = 'a D.w) : Comonad = struct
  type 'a w = 'a W.w
  include C
  let duplicate = E.extend Fn.id
  let extend f = compose (C.fmap f) D.duplicate
end;;
```
