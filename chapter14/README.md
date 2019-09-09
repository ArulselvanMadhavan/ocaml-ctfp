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
- Structure preserving mapping to Set is called a *representation*.
- Any functor F that is naturally isomorphic to hom-functor for some choice of a is called representable functor.
- For F to be representable,
  - an object a in C
  - Nat Transformation(between functors) - alpha - C(a, -) to F
  - Nat Transformation - beta - F to C(a, -)
  - Composition of these two Nat Trans is Identity Nat Trans
  - alphaAtx :: C(a, x) -> Fx  
```ocaml
module type NT_AX_FX = sig
  type a
  type 'x t
  type r = {f : 'x. a -> 'x}
  val alpha : r -> 'x t
end
```
- Pseudo OCaml expressing function equality
```OCaml
compose (F.fmap f) NT.alpha = compose NT.alpha (F.fmap f)
```
- Replacing fmap with function composition (since fmap on f is a reader functor on the right hand side)
```OCaml
F.fmap f (N.alpha h) = N.alpha (compose f h)
```
- Other Nat Transformation that goes opposite direction 
```ocaml
module type NT_FX_AX = sig
  type a
  type 'x t
  val beta : 'x t -> (a -> 'x)
end
```
- alpha . beta = id = beta . alpha
- Yoneda Lemma: Nat Trans from C(a, -) to any Set-valued functor always exists but it's not always invertible.
- Alpha example
```ocaml
module NT_Impl(F: Functor with type 'a t = 'a list) : NT_AX_SetX with type a = int and type 'x t = 'x list = struct
  type a = int
  type 'x t = 'x list
  let alpha: 'x. (int -> 'x) -> 'x list = fun h -> F.fmap h [12]
end
```
- Naturality condition is equivalent to the composiability of map.
```OCaml
F.fmap f (F.fmap h [12]) = F.fmap (compose f h) [12]
```
- Beta with example on List and Int
```ocaml
# module type NT_ListX_IntX = NT_FX_AX with type a = int and type 'x t = 'x list
```
- Beta can't be implemented for List and Int combination. (When List is empty we can't return a value of type x)
- So, List functor is not Representable.
- Rep. functors are containers for memoized results of function calls
- Representing type 'a' of C(a, -) is the key type to access values of a function.
- *alpha* is called tabulate and *beta* is called index.
- Representable functor
```ocaml
module type Representable = sig
  type 'x t
  type rep (* Representing type 'a' *)
  val tabulate : (rep -> 'x) -> 'x t
  val index : 'x t -> (rep -> 'x)
end
```
- Stream type
```ocaml
type 'a stream = | Cons of 'a * 'a stream Lazy.t
```
- Representable functor Instance
```ocaml

```
