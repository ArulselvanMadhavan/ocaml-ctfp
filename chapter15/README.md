# The Yoneda Lemma
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
```
- Yoneda Lemma is a statement about categories that doesn't have a parallel in other branches of mathematics
- Arbitrary category C, Functor F from C to Set 
- Some Set-valued functors are "representable"
- YL: All set-valued functors can be obtained from hom-functors through natural transformations and it explicitly enumerates all such natural transformations.
- Naturality condition can be quite restrictive
- YL: NT between a hom-functor and any other functor F is completely determined by specifying the value of its single component at just one point.
- Hom-functor: C(a,x) , C(a, f)
- Set-valued functor F
- alpha - NT between these two functors
- Nat Trans |C(a,-), F| = Fa
- Reader = Hom-Functor
```ocaml
type ('a, 'x) reader = 'a -> 'x
```
- Reader Functor Instance (Maps morphisms by precomposition
```ocaml
module ReaderFunctor(T : sig type a end):Functor = struct
  type 'x t = (T.a, 'x) reader
  let fmap : ('x -> 'y) -> 'x t -> 'y t = fun f r -> fun a -> f (r a)
end
```
- YL: Reader functor can be naturally mapped to any other functor
- A Polymorphic function `alpha`
```ocaml
module type NT_AX_FX = sig
  type a
  type 'x t
  val alpha : (a -> 'x) -> 'x t
end
```
- Polymorphic function 
```ocaml
val alpha : (a -> 'x) -> 'x t
```
- YL can produce a container of F a when given a polymorphic function `alpha`. (Pseudo OCaml)
```OCaml
alpha id : 'a f
```
- Converse is also true
```OCaml
val fa : 'a f
```
- Converse implementation (Define a polymorphic function from F a . (Pseudo OCaml)
```OCaml
alpha h = F.fmap h fa
```
- Advantage of having a multiple representations is that one might be easier to compose than the other or one might be efficient than the other.
- CPS = YL applied to Identity Functor
### Co-Yoneda
- YL on C^op 
- Nat|C(-, a), F) = F a
```
forall: 'x . ('x -> 'a) -> 'x t = 'a f
```
