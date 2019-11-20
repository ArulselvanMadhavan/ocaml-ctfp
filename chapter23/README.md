# Comonads
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
let id a = a
let compose f g x = f (g x)
type ('s, 'a) prod = Prod of 'a * 's
type ('s, 'a) reader = Reader of ('s -> 'a)
```
## Introduction
- Monad - composing kleisli arrows
```OCaml
'a -> 'b m
```
- Comonad
```OCaml
'a w -> 'b
```
- CoKleisli arrow - analog of the fish operator
```ocaml
module type CoKleisli = sig
  type 'a w
  val (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c)
end
```
- CoKleisli - identity arrow - dual of return
```OCaml
val extract : 'a w -> 'a
```
- Comonad type
```ocaml
module type Comonad = sig
  type 'a w
  include Functor with type 'a t := 'a w
  val extract : 'a w -> 'a
  val (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c)
end
```
- Reader monad dissection
```OCaml
'a * 'e -> 'b
```
- With currying
```OCaml
'a -> ('e -> 'b)
```
- They already have the co-Kleisli arrows
```ocaml
type ('e, 'a) product = P of 'e * 'a
```
- Implementing composition for product
```ocaml
let (=>=) f g = fun (P (e, a)) ->
  let b = f (P (e, a)) in
  let c = g (P (e, b)) in
  c
```
- Implementing extract for product
```ocaml
let extract (P (e, a)) = a
```
- Product comonad can be used to perform exactly the same computations as the reader monad.
- Reader functor is the right adjoint of the product functor
### Dissecting the composition
```OCaml
module CoKleisliImpl = struct
type 'a w
let (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c) = fun f g ->
  g ...
end
```
- Dual of bind is extend
```OCaml
val extend : ('a w -> 'b) -> 'a w -> 'b w
```
- Implementing composition using extend
```ocaml
module type CoKleisliExtend = sig
  type 'a w
  val extend : ('a w -> 'b) -> 'a w -> 'b w
end
module CoKleisliImpl(C : CoKleisliExtend) = struct
  type 'a w = 'a C.w
  let (=>=) : ('a w -> 'b) -> ('b w -> 'c) -> ('a w -> 'c) = fun f g ->
    compose g (C.extend f)
end
```
- Duplicate
```OCaml
val duplicate : 'a w -> 'a w w
```
- Three equivalent definitions of co-monad - Co-Kleisli arrows, extends or duplicate
```ocaml
module type ComonadBase = sig
  type 'a w
  include Functor with type 'a t = 'a w
  val extract : 'a w -> 'a
end

module type ComonadDuplicate = sig
  type 'a w
  val duplicate : 'a w -> 'a w w
end

module type ComonadExtend = sig
  type 'a w
  val extend : ('a w -> 'b) -> 'a w -> 'b w
end

module type Comonad = sig
  type 'a w
  include ComonadBase with type 'a w := 'a w
  include ComonadExtend with type 'a w := 'a w
  include ComonadDuplicate with type 'a w := 'a w
end

(* Construct a full comonad instance using one of the following modules *)
module rec ComonadImplViaExtend: functor(C:ComonadBase)(D:ComonadDuplicate with type 'a w = 'a C.w) -> Comonad with type 'a w = 'a C.w = functor(C:ComonadBase)(D:ComonadDuplicate with type 'a w = 'a C.w) -> struct
  include C
  include D
  let extend f wa = (C.fmap f) (D.duplicate wa)
end and ComonadImplViaDuplicate: functor (C:ComonadBase)(E:ComonadExtend with type 'a w = 'a C.w) -> Comonad with type 'a w = 'a C.w = functor(C:ComonadBase)(E:ComonadExtend with type 'a w = 'a C.w) -> struct
  include C
  include E
  let duplicate (wa : 'a w):'a w w = E.extend id wa
end
```
### Stream comonad
```ocaml
type 'a stream = Cons of 'a * 'a stream Lazy.t;;
```
- Functor instance
```ocaml
module StreamFunctor : Functor with type 'a t = 'a stream = struct
  type 'a t = 'a stream
  let rec fmap f = function
    | Cons (x, xs) -> Cons (f x, Lazy.from_val (fmap f (Lazy.force xs)))
end
```
- Get the first element of stream - extract
```ocaml
let extract (Cons (x, _)) = x
```
- Duplicate produces stream of streams
```ocaml
let rec duplicate (Cons (x, xs)) = Cons (Cons (x, xs), Lazy.from_val (duplicate (Lazy.force xs)))
```
- Complete comonad instance
```ocaml
(* Implement Extract *)
module StreamComonadBase(F:Functor with type 'a t = 'a stream):ComonadBase with type 'a w = 'a stream = struct
  type 'a w = 'a stream
  include F
  let extract (Cons (x, _)) = x
end

(* Implement duplicate *)
module StreamComonadDuplicate : ComonadDuplicate with type 'a w = 'a stream = struct
  type 'a w = 'a stream
  let rec duplicate (Cons (x, xs)) = Cons (Cons (x, xs), Lazy.from_val (duplicate (Lazy.force xs)))
end

(* Generate full Comonad Instance *)
module StreamComonad : Comonad with type 'a w = 'a stream = ComonadImplViaExtend(StreamComonadBase(StreamFunctor))(StreamComonadDuplicate)
```
- Analog of advance
```ocaml
let tail (Cons (_, xs)) = Lazy.force xs
```
- sum
```ocaml
let rec sum_s n (Cons (x, xs)) = 
  if n <= 0 then 0 else x + sum_s (n - 1) (Lazy.force xs)
```
- average
```ocaml
let average n stm = Float.(of_int (sum_s n stm) /. of_int n)
```
- movingAverage
```ocaml
let moving_average n = StreamComonad.extend (average n)
```
### Comonad Categorically
- NT reversed for comonad. E : T -> I and D : T -> T^2
- components of these transformations correspond to extract and duplicate
- Monad can be derived from adjunction - R `compose` L - Monad
- L `compose` R - Comonad
- counit of the adjunction - E : L `compose` R -> I - extract
- D : L `compose` R `compose` L `compose` R - duplicate
- monad is a monoid
- Is Comonad a `comonoid`
- `monoid` - an object in the monoidal category
- U : m * m -> m
- N : i -> m
- Comonoid - Reversing the above morphisms
- D : m -> m * m
- E : m -> i
```ocaml
module type Comonoid = sig
  type m
  val split : m -> m * m
  val destroy : m -> unit
end
```
- destroy
```ocaml
let destroy _ = ()
```
- split
```OCaml
let split x = (f x, g x)
```
- Comonoid laws dual to monoid laws
```OCaml
(* lambda is the left unitor and rho is the right unitor *)
(* <.> is used as compose below *)
lambda <.> (bimap destroy id) <.> split = id
rho <.> (bimap id destroy) <.> split = id
```
- Plugging in the definitions
```OCaml
lambda (bimap destroy id (split x))
  = lambda (bimap destroy id (f x, g x))
  = lambda ((), g x)
  = g x
```
- So we can conclude that g = id and f = id
- split becomes
```ocaml
let split x = (x, x)
```
- Every object is a trivial comonoid
- Monad is a monoid in the category of endofunctors
- Comonad is a Comonoid in the category of endofunctors
### Store Comonad
- Dual of a state monad - store comonad
- L z = z * s
- R a = s => a
- For costate(Store) comonad
- Comonad - L `compose` R
- L (R a) = (s => a) * s
- Prod as L and Reader as R
```ocaml
type ('s, 'a) store = Store of (('s -> 'a) * 's)
```
- counit of the adjunction Ea : ((s => a) * s) -> a
```ocaml
let counit (Prod (Reader f, s)) = f s
```
- extract function
```ocaml
let extract (Store (f, s)) = f s
```
- unit of adjunction
```ocaml
let unit a = Reader (fun s -> Prod (a, s))
```
- Partial construction of Store
```ocaml
let make_store f = fun s -> Store (f, s)
```
- duplicate - D : L `compose` R -> L `compose` R `compose` L `compose` R -> L `compose` Eta `compose` R
```ocaml
let duplicate (Store (f, s)) = Store (make_store f, s)
```
- complete defintion
```ocaml
module StoreComonadBase(S: sig type s end)(F: Functor with type 'a t = (S.s, 'a) store):ComonadBase with type 'a w = (S.s, 'a) store = struct
  type 'a w = (S.s, 'a) store
  include F
  let extract (Store (f, s)) = f s
end

module StoreComonadDuplicate(S: sig type s end) : ComonadDuplicate with type 'a w = (S.s, 'a) store = struct
  type 'a w = (S.s, 'a) store
  let duplicate (Store (f, s)) = Store (make_store f, s)
end

(* Generate Full comonad *)
module StoreComonad(S : sig type s end)(F:Functor with type 'a t = (S.s, 'a) store) : Comonad with type 'a w = (S.s, 'a) store = ComonadImplViaExtend(StoreComonadBase(S)(F))(StoreComonadDuplicate(S));;
```
- Reader of the store - generalized container of `a`s that are keyed using elements of type s
- Second argument of the store - current position in the stream
- duplicate - creates an infinite stream indexed by an element of type s
- Basis for Lens
```OCaml
'a -> ('s, 'a) store
```
- Equivalent to
```OCaml
val get : 'a -> 's
val set : 'a -> 's -> 'a
```
