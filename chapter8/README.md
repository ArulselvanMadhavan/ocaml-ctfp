# Functoriality
### Bifunctors
- Functors are morphisms in Cat.
- Bifunctors - Functors of two arguments.
- Bifunctor maps every pair of objects, one from category C and one from category D, to category E.
- Bifunctors map morphism from C and morphism from D to a morphism in E.
- Pair of objects is an object in the category C x D and a pair of morphism is just a morphism in the category C x D.
- Bifunctors - Functors in both arguments.
- Bifunctor using recursive modules.
- Signature 1
```ocaml
module type BifunctorCore = sig
  type ('a, 'b) t
  val bimap : ('a -> 'c) -> ('b -> 'd) -> ('a, 'b) t -> ('c, 'd) t
end
```
- Signature 2
```ocaml
module type BifunctorExt = sig
  type ('a, 'b) t
  val first : ('a -> 'c) -> ('a, 'b) t -> ('c, 'b) t
  val second : ('b -> 'd) -> ('a, 'b) t -> ('a, 'd) t
end
```
- Definition 1
```ocaml
module BifunctorCore_Using_Ext(M : BifunctorExt):BifunctorCore = struct
  type ('a, 'b) t = ('a, 'b) M.t
  let bimap g h x = M.first g (M.second h x)
end
```
- Definition 2
```ocaml
module BifunctorExt_Using_Core(M : BifunctorCore):BifunctorExt = struct
  type ('a, 'b) t = ('a, 'b) M.t
  external id : 'a -> 'a = "%identity"
  let first g x = M.bimap g id x
  let second h x = M.bimap id h x
end
```
- Recursive Definition Example
```ocaml
module rec BifunctorCoreImpl:BifunctorCore = BifunctorCore_Using_Ext(BifunctorExtImpl)
and BifunctorExtImpl:BifunctorExt = BifunctorExt_Using_Core(BifunctorCoreImpl)
```
