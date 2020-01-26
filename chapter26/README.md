# Ends and Coends
### Utilities used by code below
### Introduction
- Morphism : a -> b
- a and b are related
- Existence of the morphism is a proof of this relation
- poset category
  - morphism is a relation
- There may be mant proofs of the same relation between two objects
- These proofs form a set called hom-set
- Vary the objects, we get a mapping from pairs of objects to sets of "proofs"
- Hom Functor : C(-, =) :: C^op x C -> Set
- A relation may also involve two different categories C and D
- p :: D^op x C -> Set
- A profunctor from C to D
```ocaml
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
module type Profunctor = sig
  type ('a,'b) p
  val dimap : ('c -> 'a) -> ('b -> 'd) -> ('a, 'b) p -> ('c, 'd) p
end
let id a = a
```
- Functoriality of the profunctor tells us that if we have a proof that a is related to b then we get the proof that c is related to d
  - as long as there is a morphism from c to a
  - another from b to d
```OCaml
let dimap f id (P (b, b)) : ('a, 'b) p
```
- p (a, a) to p (a, b)
```OCaml
let dimap id f (P (a, a)) : ('a, 'b) p
```
### Dinatural transformations
  - Profunctors are functors.
  - NT between them in the standard way
  - Dinatural transformation between two profunctor p and q which are members of the functor category [C^op x C, Set] is a family of morphisms
  - f : a -> b
  - alpha_a : p a a -> q a a
  - p b a -> p f id_a -> p a a
  - p b a -> p id_b f -> p b b
  - p a a -> alpha_a -> q a a
  - p b b -> alpha_a -> q b b
  - q a a -> q id_a f -> q a b
  - q b b -> q f id_b -> q a b
  - This commuting condition is strictly weaker than naturality condition
  - Natural transformation alpha in [C^op x C, Set] is indexed by a pair of objects
  - Dinatural transformation is indexed by only one object 
### Ends
  - Calculus of category theory
  - Calculus of ends and coends borrows ideas and notation from calculus
  - coend - infinite sum or integral
  - end - infinite product
  - End is the generalization of a limit
```OCaml
(* There is no compose operator in OCaml *)
compose (dimap id f) (alpha) = compose (dimap f id) alpha
```
  - End formula
```OCaml
'a. ('a, 'a) p
```
  - Wedge condition
```OCaml
compose (dimap f id) pi = compose (dimap id f) pi
```
  - Profunctor requirement
```ocaml
module type Polymorphic_Projection = functor(P : Profunctor) -> sig
  type rank2_p = { p : 'c. ('c, 'c) P.p }
  val pi : rank2_p -> ('a, 'b) P.p
end
```
  - pi is the polymorphic projection
```ocaml
module Pi(P : Profunctor) = struct
  type rank2_p = { p : 'a. ('a, 'a) P.p }
  let pi : 'c. rank2_p -> ('c, 'c) P.p = fun e -> e.p
end
```
  - Generalization of a constant functor to a constant profunctor
    - maps all pairs of objects to a single object c
    - maps all pairs of morphisms to identity morphism
  - A wedge is a dinatural transformation from that functor to the profunctor p
  - Dinatural hexagon to wedge diamond 
### Ends as Equalizers
  - Commutation conditions as equalizer.
```ocaml
module EndsEqualizer(P : Profunctor) = struct
  let lambda : 'a 'b. ('a, 'a) P.p -> ('a -> 'b) -> ('a, 'b) P.p = fun paa f -> P.dimap id f paa
  let rho : 'a 'b. ('b, 'b) P.p -> ('a -> 'b) -> ('a, 'b) P.p = fun pbb f -> P.dimap f id pbb
end
```
  - prod p
```ocaml
module type ProdP = sig
  type ('a, 'b) p
  type ('a, 'b) prod_p = ('a -> 'b) -> ('a, 'b) p
end
```
  - diaprod
```ocaml
module type DiaProd = sig
  type ('a, 'b) p
  type 'a diaprod = DiaProd of ('a, 'a) p
end
```
  - mappings from this prod 
```ocaml
module EndsWithDiaProd(P : Profunctor)(D : DiaProd with type ('a, 'b) p = ('a, 'b) P.p)(PP : ProdP with type ('a, 'b) p = ('a, 'b) P.p) = struct
  module E = EndsEqualizer(P)
  let lambdaP : 'a 'b. 'a D.diaprod -> ('a, 'b) PP.prod_p = fun (DiaProd paa) -> E.lambda paa
  let rhoP : 'a 'b. 'b D.diaprod -> ('a, 'b) PP.prod_p = fun (DiaProd pbb) -> E.rho pbb
end
```
### Natural Transformations as Ends
```ocaml
(* Higher rank types can be introduced via records *)
module NT_as_Ends(F : Functor)(G : Functor) = struct
    type set_of_nt = { nt : 'a. 'a F.t -> 'a G.t}
end
```
  - Naturality follows from parametricity
  - Coend
```ocaml
 module Coend(P : Profunctor) = struct
  type coend = Coend of {c : 'a. ('a, 'a) P.p }
end
```
  - Coequalizer
  - cowedge conditions can be summarized 
```ocaml
module type SumP = sig
  type a
  type b
  type ('a, 'b) p
  val f : b -> a
  val pab : (a, b) p
end
```
```ocaml
module type DiagSum = sig
  type a
  type ('a, 'b) p
  val paa : (a, a) p
end

module CoEndImpl(P : Profunctor) = struct
  type a
  type b
  module type Sum_P = SumP with type ('a, 'b) p = ('a, 'b) P.p and type a = a and type b = b
  let lambda (module S : Sum_P) = 
  (module struct type a = S.b type ('a, 'b) p = ('a, 'b) P.p  let paa = P.dimap S.f id S.pab end : DiagSum)
  let rho (module S : Sum_P) =
  (module struct type a = S.a type ('a, 'b) p = ('a, 'b) P.p let paa = P.dimap id S.f S.pab end : DiagSum)
end
```
- DiagSum
```ocaml
module type DiagSum = sig
  type a
  type ('a, 'b) p
  val paa : (a, a) p
end
```
- Profunctor Composition
```ocaml
module type Procompose = sig
  type ('a, 'b) p
  type ('a, 'b) q
  type (_, _) procompose = 
    | Procompose : (('a, 'c) q -> ('c, 'b) p) -> ('a, 'b) procompose
end
```
