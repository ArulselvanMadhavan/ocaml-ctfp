# Adjunctions
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
module type Representable = sig
  type 'x t
  type rep (* Representing type 'a' *)
  val tabulate : (rep -> 'x) -> 'x t
  val index : 'x t -> (rep -> 'x)
  val fmap : ('x -> 'y) -> 'x t -> 'y t
end
```
## Equality vs Iso
- Equality is too strong
- Isomorphism - two things can be same without actually being equal as long as there is an invertible morphism
```ocaml
let swap (a, b) = (b, a)
```
## Adjunction and Unit/Counit Pair
- Categories being isomorphic is expressed in terms of functors between them.
- Cat C and D are isomorphic, 
  - if there exists a functor R from C to D
  - if there exists a functor L from D to C
- Composition of two functors R compose L and L compose R.
- R compose L = I_D
- L compose R = I_C
- Equality of functors can be expressed via natural isomorphisms.
- unit :: I_D -> R compose L
- counit :: L compose R -> I_C
- L - Left adjoint
- R - Right adjoint
- Compact Notation for Adjunction: L -| R
- unit lets us introduce composition R compose L in place of an Identity functor on D
- counit lets us eliminate composition L compose R and replace it with an Identity functor on C.
- unit in OCaml
```ocaml
module type Unit_Example = sig
  type 'a m
  val return : 'd -> 'd m
end;;
```
- extract in OCaml
```ocaml
module type Counit_Example = sig
  type 'c w
  val extract : 'c w -> 'c
end
```
- 'm' is the endofunctor corresponding to R compose L and w is the endofunctor corresponding to L compose R.
- unit is a polymorphic function that creates a default box around a value of arbitrary type.
- counit does the opposite.
- Pair of adjoint functors define a monad and comonad
- Adjunctions of Endofunctors.
```ocaml
(* L is Functor F and R is Representable Functor U *)
module type Adjunction = functor (F : Functor) (U : Representable) -> sig
  val unit : 'a -> ('a F.t) U.t
  val counit : ('a U.t) F.t -> 'a
end
```
## Adjunctions and Hom-Sets
- Adjunctions in terms of natural isomorphisms of hom-sets.
- Unique morphism - mapping some set to hom-set
- Factorization can be described in terms of NT
- Factorization involves commuting diagrams since it defines a morphism being equal to composition of two morphisms.
- Universal construction - involves factorization - morphism to commuting diagram and then to a unique morphism.
- If this morphism is invertible and it can be extended to all hom-sets then we have an adjunction.
- Alternative defintion:
  - L :: D -> C and R :: C -> D
  - d - source object in D; c - target object in C
  - Ld -> map d to C
  - hom-set between Ld and c = C(Ld, c)
  - Rc -> map c to R
  - hom-set between d and Rc = D(d, Rc)
  - L is left-adjoint to R iff there is an isomorphism of hom-sets:
    - C(Ld, c) is isomorphic to D(d, Rc)
  - Naturality means d can be varied across D and c can be varied across C.
```ocaml
module type Adjunction_HomSet = functor (F : Functor)(U : Representable) -> sig
  val left_adjunct : ('a F.t -> 'b) -> ('a -> 'b U.t)
  val right_adjunct : ('a -> 'b U.t) -> ('a F.t -> 'b)
end
```
- Equivalence between unit/counit and left_adjunct/right_adjunct
```OCaml
let unit = left_adjunct id
let counit = right_adjunct id
let left_adjunct f a = f
```
- Complete Adjunction Definition
```ocaml
module type Adjunction = functor (F : Functor)(U : Representable) -> sig
  val unit : 'a -> ('a F.t) U.t
  val counit : ('a U.t) F.t -> 'a
  val left_adjunct : ('a F.t -> 'b) -> ('a -> 'b U.t)
  val right_adjunct : ('a -> 'b U.t) -> ('a F.t -> 'b)  
end

module type Adjunction_Unit_Counit = functor(F: Functor)(U: Representable) -> sig
  val unit : 'a -> ('a F.t) U.t
  val counit : ('a U.t) F.t -> 'a
end

module type Adjunction_Hom_Set = functor (F:Functor)(U:Representable) -> sig
  val left_adjunct : ('a F.t -> 'b) -> 'a -> 'b U.t
  val right_adjunct : ('a -> 'b U.t) -> 'a F.t -> 'b
end

(* Implementing Adjunction from Hom_Set Definitions *)
module Adjunction_From_Hom_Set(A : Adjunction_Hom_Set) : Adjunction = functor(F : Functor)(U : Representable) -> struct
  type 't f = 't F.t
  type 't u = 't U.t
  module M = A(F)(U)
  include M
  let unit : 'a. 'a -> 'a f u = fun a -> M.left_adjunct idty a
  let counit : 'a. ('a u) f -> 'a = fun fua -> M.right_adjunct idty fua
end

(* Implementing Adjunction from unit/counit definitions *)
module Adjunction_From_Unit_Counit(A:Adjunction_Unit_Counit):Adjunction = functor(F:Functor)(U:Representable) -> struct
  type 't f = 't F.t
  type 't u = 't U.t
  module M = A(F)(U)
  include M
  let left_adjunct f = fun a -> (U.fmap f) (M.unit a)
  let right_adjunct f = fun fa -> M.counit (F.fmap f fa)
end
```
- Why is the Right adjoint a representable functor
  - Right category D is Set
  - R is representable if we can find an object rep in C such that C(rep, _) is naturally isomorphic to R
  - rep = L()
  - C(L(), c) is naturally isomorphic to Set((), Rc)
  - C(L(), _) is naturally isomorphic to R
  - So, R is representable
### Product from Adjunction
- factorizer
```ocaml
let factorizer p q = fun x -> (p x, q x)
```
- Pseudo OCaml expressing function equality
```OCaml
compose fst (factorizer p q) = p
compose snd (factorizer p q) = q
```
- Product category
  - C and D
  - C x D - product category - pairs of objects from C and D - pairs of morphisms from C and D
```OCaml
int * bool ~ (int, bool)
```
## Exponential form Adjunction
- 
