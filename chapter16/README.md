# Yoneda Embedding
## Utilities used by code below
```ocaml
let id a = a
```
- C(a, -) - covariant functor from C to Set
- For every a, when we try to get a mapping from C(a, -) to Set, they end up being objects in the functor category.
- [C, Set] Functor Category from C to Set
- A morphism in C is C(a, b)
- A morphism in [C, Set] is a natural transformation.
- Mapping of morphism to Natural Transformations is interesting
- a is mapped to C(a, -) 
- b is mapped to C(b, -)
- YL: [C, Set](C(a,-), F) = Fa
- [C, Set](C(a, -), C(b,-)) = C(b, a)
- NT between two hom-functors we got using YL gave us a contravariant functor
- Mapping from C to [C, Set] is a contravariant, fully faithful functor
- Faithful functor - Injective on hom-sets - No coalescing of morphisms
- Full functor - surjective on hom-sets - maps one hom-set onto the other hom-set, fully covering the latter.
- Fully faithful functor is a bijection on hom-sets.
### Embedding
- The contravariant functor that maps objects in C to [C, Set] defines the Yoneda Embedding
- It embeds C inside [C, Set]
- Co-Yoneda Embedding
  - Embedding of category C in the category of presheaves
  - [C, Set](C(-, a), C(-, b)) = C(a, b)
### Application to OCaml
- Yoneda Embedding - isomorphism between natural transformations amongst reader functors and functions
```ocaml
module type BtoA = sig
  type a
  type b
  val btoa : b -> a
end
(* Define the Yoneda embedding *)
module Yoneda_Embedding(E: BtoA) = struct
  let fromY : (E.a -> 'x) -> E.b -> 'x = fun f b -> f (E.btoa b)
end
```
- To recover converter
```OCaml
module YE = Yoneda_Embedding(BtoAImpl)
YE.fromY id (* output type : BtoA.b -> BtoA.a *)
```
### Preorder example
- YE to preorder category
- A set with Preorder relation gives rise to a category.
- hom-set in this category is either an empty set or a one-element set.
## Naturality
- YL establishes the isomorphism between the set of NT and an object in Set.
- Set of NT between any functors is a hom-set in the category [C, Set]
- [C, Set](C(a,-), F) = Fa
- Natural isomorphism is an invertible NT between two functors.
