# Limits and Colimits
### Utilities used by the code below
```ocaml
let compose f g x = f (g x)
```
- In CT, everything is related to everything and can be viewed from many angles.
### Generalizing products
- 2 category
  - contains two objects.
  - No morphisms between them.
  - Only identity morphsisms.
- Selecting two objects in C is the same as defining a functor(D) from 2 to C.
- Selecting a candidate object c
  - A constant functor from 2 to C
    - Selection of c in C is done using the constant functor.
- Natural transformation between constant functor (to c) and D
  - NT component - 'c' to 'a' is a morphism - p
  - NT component - 'c' to 'b' is a morphism - q
```OCaml
val p1 = compose p m
val q1 = compose q m
```
- Contramap definition
```ocaml
# let contramap : 'c_prime 'c 'limD. ('c_prime -> 'c) -> ('c -> 'limD) -> ('c_prime -> 'limD) = fun f u -> compose u f
val contramap : ('c_prime -> 'c) -> ('c -> 'limD) -> 'c_prime -> 'limD =
  <fun>
```
- Presheaves: Contravariant functor from any category C to Set
- Natural Isomorphism: Natural Transformation whose every component is an isomorphism.
### Examples of limits
- Equalizer
```OCaml
val f : 'a -> 'b
val g : 'a -> 'b
```
```OCaml
val p : 'c -> 'a
val q : 'c -> 'b
```
```OCaml
q = compose f p
q = compose g p
```
```OCaml
(** Pseudo OCaml expressing function equality **)
compose f p = compose g p
```
```ocaml
let f (x, y) = 2 * y + x
let g (x, y) = y - x
```
```ocaml
let p t = (t, (-2) * t)
```
```OCaml
(** Pseudo OCaml expressing function equality **)
compose f p' = compose g p'
```
```ocaml
let p' () = (0, 0)
```
```OCaml
let p' = compose p h
```
```ocaml
let h () = 0
```
- Pullback
```OCaml
val f : 'a -> 'b
val g : 'c -> 'b
```
- cospan
```OCaml
val p : 'd -> 'a
val q : 'd -> 'c
val r : 'd -> 'b
```
- Commutativity conditions
```OCaml
compose g q = compose f p
```
```ocaml
let f x = 1.23
```
```ocaml
module type Contravariant = sig
  type 'a t
  val contramap : ('b -> 'a) -> 'a t -> 'b t
end
type 'a tostring = ToString of ('a -> string)

module ToStringInstance : Contravariant = struct
  type 'a t = 'a tostring
  let contramap f (ToString g) = ToString (compose g f)
end
```
```OCaml
('b 'c either) tostring ~ ('b -> string, 'c -> string)
```
```OCaml
'r -> ('a, 'b) ~ ('r -> 'a, 'r -> 'b)
```

