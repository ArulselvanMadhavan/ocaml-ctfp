# Representable Functors
## Utilities used by the code below
```ocaml
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
let flip f b a = f a b
let compose f g x = f (g x)
```
- Set theory - assembly language of mathematics
- *Set* category of all sets
- morphisms between any two objects in a category form a set (hom-set)
- All the information about sets can be encoded in *isormorphic* functions between them.
- Interaction between programming and math goes both ways.
## Hom Functor
- There's family of mappings from a category to Set
- These mappings are functors.
- hom-functor is reader functor
```ocaml
type ('a, 'x) reader = 'a -> 'x
```
- C(a,-) - CoVariant Functor instance
```ocaml
module ReaderFunctor(T : sig type r end) : Functor = struct
  type 'a t = (T.r, 'a) reader
  let fmap f h = fun a -> f (h a)
end
```
- C(-,a) - Contravariant Functor
```ocaml
type ('a, 'x) op = 'x -> 'a
```
- Contravariant instance
```ocaml
module OpContravariant(T : sig type r end) : Contravariant = struct
  type 'a t = (T.r, 'a) op
  let contramap f h = fun b -> h (f b)
end
```
- C(-, -) - If we allows both arguments to change
```ocaml
module ProfunctorArrow : Profunctor = struct
  type ('a, 'b) p = 'a -> 'b
    let dimap f g p = compose g (compose p f)
    let lmap f p = (flip compose) f p
    let rmap = compose
end
```
- The mapping of objects from any category to hom-sets is functorial.
- C^op x C -> Set
## Representable Functors
- 
