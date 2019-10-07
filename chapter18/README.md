# Adjunctions
## Utilities used by code below
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
module type Adjunction = functor (F : Functor) (U : Representable) -> sig
  val unit : 'a -> ('a F.t) U.t
  val counit : ('a U.t) F.t -> 'a
end
```
    
