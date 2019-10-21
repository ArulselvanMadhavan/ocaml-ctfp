# Free/Forgetful Adjunctions
## Utilities used by code below
## Introduction
- A free functor is the left adjoint of a forgetful functor
- Forgetful functor - that forgets some structure
- A functor mapping from C to Set.
- A set corresponding to some object in C is called underlying set.
- Monoid category has objects that have underlying sets
- Forgetful functor U -> maps from Mon to Set
- U maps monoid homomorphisms to functions between sets.
- Mon to Set
- Adjunction: Mon(Fx, m) = Set(x, Um)
- Fx is the maximum monoid that can be built on the basis of x.
## Monoid with list type
- 'a list is a free monoid where 'a represents the set of generators
- Appending is associative and unital
```ocaml
(* OCaml's string type is an immutable array of bytes *)
type string' = char list
```
- Isomorphism with list of units
```ocaml
let to_nat : unit list -> int = List.length
let to_lst : int -> unit list = fun n -> List.init n ~f:(fun _ -> ())
```
## Intuitions
- Functors and functions are lossy in nature.
- Functors may collapse multiple objects and morphisms
- Functions may collapse multiple elements of a set.
- Monomorphisms - Injective(non-collapsing)
- Epimorphisms - Surjective(covering the whole codomain)
- Free -| Forgetful adjunction
  - Constrained category C is on the left
  - Less constrained category D is on the right
- Morhphisms in C are fewer because they have to preserve additional structure.
- Image of F must consist of structure-free objects, so that there is no structure to preserve by morphisms.
- These objects of F are called free objects.
- C(Fd, c) = D(d, Uc)
- Free monoids instead of performing the operation, they accumulate the arguments that were passed to it.
- Free constructions: Accumulate expression trees before evaluating them.
