# F-Algebras
### Utilities used by code below
```ocaml
module type Algebra = functor (F : sig type 'a f end) -> sig
  type 'a algebra = 'a F.f -> 'a
end
module Algebra : Algebra = functor (F : sig type 'a f end) -> struct
  type 'a algebra = 'a F.f -> 'a
end
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
```
### Introduction
- Monoid - set, single object category, object in monoidal category
- F Algebra
```ocaml
module type Algebra = functor(F : sig type 'a f end) -> sig
  type 'a algebra = 'a F.f -> 'a
end
```
- MonoidF functor
```ocaml
type 'a mon_f = MEmpty | Mappend of ('a * 'a)
```
- RingF functor
```ocaml
type 'a ring_f = RZero 
| ROne 
| RAdd of ('a * 'a) 
| RMul of ('a * 'a) 
| RNeg of 'a
```
- evalZ function
```ocaml
module Ring = struct

  module RingAlg = Algebra(struct type 'a f = 'a ring_f end)

  let eval_z : 'a RingAlg.algebra = function
    | RZero -> 0
    | ROne  -> 1
    | RAdd (m, n) -> m + n
    | RMul (m, n) -> m * n
    | RNeg n -> -n
end
```
- Recursion
```ocaml
type expr =
    RZero
  | ROne
  | RAdd of (expr * expr)
  | RMul of (expr * expr)
  | RNeg of expr
```
- Ring Evaluator with a recursive definition
```ocaml
  let rec eval_z : expr -> int = function
    | RZero -> 0
    | ROne  -> 1
    | RAdd (e1, e2) -> eval_z e1 + eval_z e2
    | RMul (e1, e2) -> eval_z e1 * eval_z e2
    | RNeg e -> -(eval_z e)
```
- Depth-one tree
```ocaml
type 'a ring_f1 = ('a ring_f) ring_f
```
- Depth two tree
```ocaml
type 'a ring_f2 = (('a ring_f) ring_f) ring_f
```
- D2 via D1
```ocaml
type 'a ring_f2 = 'a ring_f ring_f1
```
- Applying an endofunctor infinitely many times produces a fixed point
```ocaml
module Fix(F : Functor) = struct
    type 'a fix = Fix of (('a fix) F.t)
end
```
- Constructor name - Fix is a convention
```ocaml
module Fix(F : Functor) = struct
    type 'a fix = In of (('a fix) F.t)
end
```
- Fix as a function
```ocaml
module Fix(F : Functor) = struct
    type 'a fix = Fix of (('a fix) F.t)
    let fix : 'a. 'a fix F.t -> 'a fix = fun f -> Fix f
end
```
- unfix
```ocaml
module Fix(F : Functor) = struct
    type 'a fix = Fix of (('a fix) F.t)
    let unfix : 'a.'a fix -> 'a fix F.t = fun (Fix f) -> f
end
```
### Category of F-Algebras
- F Algebras form a category
- carrier object : a
- morphism : f : F a -> a
- Objects in that category are a pair (a, f)
- Morphisms in the F-algebra category : (a, f) -> (b, g)
  - m: a -> b
- Homomorphism of F-algebras
  - F m : F a -> F b
  - g : F b -> b
  - g compose F m = m compose f
- Initial Algebra
  - carrier i
  - j :: F i -> i
- Lambek's theorem : j is an isomorphism
- There is a unique homomorphism m from initial object to any other F-algebra
- j : F i -> i ; m : j -> i ; F m : F i -> F a ; f : F a -> a
- A new algebra
  - Carrier : F i
  - morphism : F j : F (F i) -> F i
- (i, j) is the initial algebra. Unique homomorphism 'm' must connect initial algebra (i, j) with (F i, F j)
- j : F i -> i ; m : i -> F i ; F m : F i -> F (F i); F j : F (F i) -> F i
- A new algebra 
  - F j : F (F i) -> F i
  - j : F i -> i
- j is a homomorphism of algebras (F i, F j) to (i, j)
- j compose m is a homomorphism of two algebras (i, j) and (F i, F j)
  - i compose m = id_i
- m compose j = id_Fi
- i is the inverse of m and m is the inverse of j
- j is an isomorphism between F i and i
- j is the constructor Fix
- i is the Fix f
- m is the inverse unFix
### Natural Numbers
- zero : 1 -> N ; succ : N -> N
- 1 + N -> N
- As functor
```ocaml
type 'a nat_f = ZeroF | SuccF of 'a;;
```
- Fixed point (Initial algebra)
```ocaml
type nat = Zero | Succ of nat
```
- Peano representation for natural numbers
### Catamorphisms
- Initial algebra - Fix f
- Evaluator is the constructor Fix
- Unique morphism m : Initial algebra to any other algebra
- Algebra : carrier a and evaluator is alg
- Fix : f (Fix f) -> Fix f
- m : Fix f -> a
- fmap m (f (Fix f)) -> f a
- alg : f a -> a
- m is the evaluator of the fixed point
- m evaluates the whole expression tree
- m = alg . fmap m . unfix
- cata in OCaml
```ocaml
module Cata(F : Functor) = struct
  type 'a fix = Fix of (('a fix) F.t)
  let fix : 'a. ('a fix) F.t -> 'a fix = fun f -> Fix f
  let unfix : 'a. 'a fix -> 'a fix F.t = fun (Fix f) -> f
  let rec cata : 'a. ('a F.t -> 'a) -> 'a fix -> 'a = fun alg fixf -> alg (F.fmap (cata alg) (unfix fixf))
end
```
- Functor for natural numbers
```ocaml
type 'a nat_f = ZeroF | SuccF of 'a
```
- Carrier Type : (int, int)
- Algebra 
```ocaml
let rec fib = function 
  | ZeroF -> (1, 1)
  | SuccF (m, n) -> (n, m + n)
```
- Algebra for NatF defines the recurrence relation and the catmorphism just evaluates the n-th element of that sequence
### Folds
```ocaml
type ('e, 'a) list_f = NilF | ConsF of ('e * 'a)
```
- Replacing 'a with the result of recursion - 'e list
```ocaml
type 'e list' = Nil | Cons of ('e * 'e list')
```
- Algebra for a list functor picks a particular carrier type and defines a function that does pattern matching on the two constructors
```ocaml
let len_alg = function
  | ConsF (e, n) -> n + 1
  | NilF -> 0
```
- Traditional list length
```ocaml
let length xs = List.fold_right (fun e n -> n + 1) xs 0
```
- Two arguments to fold_r are the two components of the algebra
```ocaml
let sum_alg = function
  | ConsF (e, s) -> e +. s
  | NilF -> 0.0
```
- sum using foldr
```ocaml
let sum xs = List.fold_right (fun e s -> e +. s) xs 0.0
```
### CoAlgebras
- Direction of the morphism is reversed
- a -> F a
- Coalgebras for a given functor also form a category
- Homomorphisms preserve the coalgebraic structure
- The terminal object (t, u) is the final coalgebra
- For every alg (a, f), there is a unique homomorphism m such that
- u : t -> F t; m : a -> t; F m : F a -> F t; f : a -> F a
- Terminal coalgebra is a fixed point of the functor
- morphism : u : t -> F t is an isomorphism
- Terminal coalgebra -> recipe for generating infinite data structures
- Cata is used to evaluate initial algebra
- Ana is used to evaluate final coalgebra
```ocaml
module Ana(F:Functor) = struct

  type 'a fix = Fix of ('a fix) F.t
  
  let rec ana : 'a. ('a -> 'a F.t) -> 'a -> 'a fix = fun coalg a -> Fix (F.fmap (ana coalg) (coalg a))
end
```
- Stream as an example
```ocaml
type ('e, 'a) stream_f = StreamF of ('e * 'a)

module Stream_Functor(E : sig type e end) : Functor with type 'a t = (E.e, 'a) stream_f = struct
  type 'a t = (E.e, 'a) stream_f
  let fmap f = function
    | StreamF (e, a) -> StreamF (e, f a)
end
```
- Fixed point
```ocaml
type 'e stream = Stream of ('e * 'e stream)
```
- Generating Sieve of eratosthenes
```ocaml
(* OCaml library Gen provides infinite data structures *)
let era : int Gen.t -> (int, int Gen.t) stream_f = 
  fun ilist ->
  let notdiv = fun p n -> (mod) n p != 0 in
  let p = Gen.get_exn ilist in
  StreamF (p, Gen.filter (notdiv p) ilist)
```
- Primes
```ocaml
module Stream_Int = Stream_Functor(struct type e = int end)
module Ana_Stream = Ana(Stream_Int)

(* The fixpoint translated to OCaml is eager in its evaluation. 
Hence, executing the following function will cause overflow.
So, wrapping it inside a lazy *)
let primes = lazy (Ana_Stream.ana era (Gen.init (fun i -> i + 2)))
```
- to_list_c
```ocaml
module List_C(E : sig type e end) = struct
  module Stream_F: Functor with type 'a t = (E.e, 'a) stream_f = Stream_Functor(E)
  module Cata_Stream = Cata(Stream_F)
  
  let to_list_c : E.e list Cata_Stream.fix -> E.e list = 
    fun s_fix ->
    Cata_Stream.cata (fun (StreamF (e, a)) -> e :: a) s_fix

end
```
- Fixed point is the initial algebra and final coalgebra
- Endofunctor may have many fixed points
- Initial algebra is the least fixed point
- Final Coalgebra is the greatest fixed point
- unfold
```OCaml
(* Gen.t is used to represent infinite data structures like haskell's lazy list *)
val unfold : ('b -> ('a * 'b) option) -> 'b -> 'a Gen.t
```
- A lens can be represented as a pair of getter and setter.
- set
```OCaml
val set : 'a -> 's -> 'a
val get : 'a -> 's
```
- a is a product type; s is the field type
```OCaml
(a, (s, s -> a))
```
```OCaml
'a -> ('s, 'a) store
```
- Functor
```ocaml
(* Store is the comonad version of State *)
type ('s, 'a) store = Store of ('s -> 'a) * 's
```
- Lens is a coalgebra for functor with carrier type a
- Lens is a coalgebra that is compatible with comonad structure 
