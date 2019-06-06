# Functoriality
### Bifunctors
- Functors are morphisms in Cat.
- Bifunctors - Functors of two arguments.
- Bifunctor maps every pair of objects, one from category C and one from category D, to category E.
- Bifunctors map morphism from C and morphism from D to a morphism in E.
- Pair of objects is an object in the category C x D and a pair of morphism is just a morphism in the category C x D.
- Bifunctors - Functors in both arguments.
```ocaml
(** You can represent bifunctor defintion in two forms and implement just and derive the other from it. *)
module type BifunctorCore = sig
  type ('a, 'b) t
  val bimap : ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) t -> ('c, 'd) t
end

Module type BifunctorExt = sig
  type ('a, 'b) t
  val first : ('a -> 'c) -> ('a, 'b) t -> ('c, 'b) t
  val second : ('b -> 'd) -> ('a, 'b) t -> ('a, 'd) t
end

module BifunctorCore_Using_Ext(M : BifunctorExt):BifunctorCore = struct
  type ('a, 'b) t = ('a, 'b) M.t
  let bimap g h x = M.first g (M.second h x)
end

module BifunctorExt_Using_Core(M : BifunctorCore):BifunctorExt = struct
  type ('a, 'b) t = ('a, 'b) M.t
  external id : 'a -> 'a = "%identity"
  let first g x = M.bimap g id x
  let second h x = M.bimap id h x
end
```
- Example of bifunctor - Product type
```ocaml
module Bifunctor_Product : BifunctorCore = struct
  type ('a, 'b) t = 'a * 'b
  let bimap : 'a 'b 'c 'd. ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) t -> ('c, 'd) t = fun f g x -> (f (fst x), g (snd x))
end
```
- bimap signature for product type.
```OCaml
val bimap : ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) Bifunctor_Product.t -> ('c, 'd) Bifunctor_Product.t
```
- Coproduct
```ocaml
type ('a, 'b) either = Left of 'a | Right of 'b
module Bifunctor_Either : BifunctorCore = struct
  type ('a, 'b) t = ('a, 'b) either
  let bimap f g = function | Left a -> Left (f a) | Right b -> Right (g b)
end
```
- Monoidal category defines 
  - a binary operator(must be a bifunctor) acting on objects.
  - a unit object
- Set is a monoidal category with respect to cartesian product.
- Set is a monoidal category with respect to disjoint union.
### Functoral ADT
- ADTs are made of sum and product types.
- Sum and Product types are functorial.
- Functors compose.
- Identity type
```ocaml
type 'a id = Id of 'a
```
- Functor for Identity.
```ocaml
module Identity_Functor : Functor = struct
  type 'a t = 'a id
  let fmap f = function | Id a -> Id (f a)
end
```
- Maybe type
```ocaml
type 'a option = None | Some of 'a
```
- None part can be represented using Const () functor.
- Just part can be represented using Id functor.
- Maybe type using Either
```ocaml
(** OCaml doesn't have a built in Const type *)
type ('a, 'b) const = Const of 'a;;
(** OCaml doesn't have a built in either type *)
type ('a, 'b) either = Left of 'a | Right of 'b
(** Either type *)
type 'a option = ((unit, 'a) const, 'a id) either
```
- Composition of functors is a functor.
- In OCaml, abstracting over type constructors requires some extra work.
```ocaml
(** OCaml doesn't support higher kinded types. So, we have to use module functors to emulate the behavior higher kinded types. There's less verbose options using type defunctionalization but it's more advanced and obscures the flow of this book *)
module type BiComp = functor(BF: sig type ('a, 'b) t end)(FU : sig type 'a t end)(GU: sig type 'b t end) -> sig
  type ('a, 'b) bicomp = BiComp of ('a FU.t, 'b GU.t) BF.t
end
```
- Bifunctor instance
```ocaml
module BiCompBifunctor(BF: BifunctorCore)(FU:Functor)(GU:Functor):BifunctorCore = struct
  type ('a, 'b) t = BiComp of ('a FU.t, 'b GU.t) BF.t
  let bimap : 'a 'b 'c 'd. ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) t -> ('c, 'd) t = fun f g -> function | BiComp x -> BiComp (BF.bimap (FU.fmap f) (GU.fmap g) x)
end
```
- type of x in the definition (Pseudo OCaml)
```OCaml
type ('a FU.t, 'b GU.t) BF.t
```
- Types of f1 and f2 (Pseudo OCaml)
```OCaml
val f1 : a -> a'
val f2 : b -> b'
```
- then final result of type bf (Pseudo OCaml)
```OCaml
val bimap : (a FU.t -> a' FU.t) -> (b GU.t -> b' GU.t) -> (a FU.t, b GU.t) -> (a' FU.t, b' GU.t)
```
- Functors
```ocaml
(** Deriving a functor in OCaml is not available as a language extension. You could try experimental library like ocsigen to derive functors.*)
type 'a tree = Leaf of 'a | Node of 'a tree * 'a tree
```
- Functor for tree
```ocaml
module TreeFunctor:Functor = struct
  type 'a t = 'a tree
  let rec fmap : 'a 'b. ('a -> 'b) -> 'a t -> 'b t = fun f -> function
    | Leaf a -> Leaf (f a)
    | Node (l, r) -> Node (fmap f l, fmap f r)
end
```
- Writer type
```ocaml
type 'a writer = 'a * string
```
- Kleisli Composition (Using Writer)
```ocaml
module KleisliComposition = struct
  let (>=>) : 'a 'b 'c. ('a -> 'b writer) -> ('b -> 'c writer) -> ('a -> 'c writer) = fun m1 m2 ->
  fun x ->
    let (y, s1) = m1 x in
    let (z, s2) = m2 y in
    (z, String.concat ~sep:"" [s1; s2])
end
```
- Kleisli Identity (Using writer)
```ocaml
module KleisliIdentity = struct
  let return : 'a -> 'a writer = fun a -> (a, "")
end
```
- Kleisli Fmap implementation - (Using Writer)
```ocaml
module KleisliFunctor : Functor = struct
  type 'a t = 'a writer
  let fmap : 'a 'b. ('a -> 'b) -> 'a t -> 'b t = fun f -> KleisliComposition.(>=>) id (fun x -> KleisliIdentity.return (f x))
end
```
