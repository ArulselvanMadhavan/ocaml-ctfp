# Monads Categorically
## Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
module type Representable = sig
  type rep (* Representing type 'a' *)
  type 'a t
  include Functor with type 'a t := 'a t
  val tabulate : (rep -> 'a) -> 'a t
  val index : 'a t -> (rep -> 'a)
end
module type MonadJoin = sig
  type 'a t
  include Functor with type 'a t := 'a t
  val join : 'a t t -> 'a t
  val return : 'a -> 'a t
end
```
- In CT, a monad is an endofunctor T equipped with a pair of natural transformations mu and eta
- mu is the NT from the square functor to T
- mu :: T² -> T
- muₐ :: T(Tₐ) -> Tₐ
- eta is the NT between Identity functor I and T
- eta :: I -> T
- etaₐ :: a -> Tₐ
- Kleisli arrow between a and b is a morphism a -> T b
- Composition of two such arrows can be implemented using mu
- f :: a -> T b
- g :: b -> T c
- Implementing composition of f and g using mu
```ocaml
module Kleisli(M : MonadJoin) = struct
  (* compose *)
  let (<.>) f g x = f (g x)

  let (>=>) f g = M.join <.> M.fmap g <.> f
end
```
- In components
```ocaml
module Kleisli(M : MonadJoin) = struct

  let (>=>) f g a = M.join (M.fmap g (f a))
end
```
- To make Kleisli arrows form a category, we want their composition to be associative and eta at a, to be the identity kleisli arrow at a
- In terms of monoid laws
  - mu is multiplication
  - eta is unit
- Monoidal categories
```ocaml
module type Monoid = sig
  type m
  val mappend : m -> m -> m
  val mempty  : m
end
```
- definition of mappend
```OCaml
val mappend : m -> (m -> m)
```
- Alternate definition of mappend
```OCaml
val mu : (m, m) -> m
```
- Alt definition of mempty
```OCaml
val eta : unit -> m
```
- Associativity in monoids
```OCaml
mu (x, mu(y, z)) = mu (mu (x, y), z)
```
- Towards point-free
  - LHS
```OCaml
(compose mu (bimap id mu))(x, (y, z))
```
  - RHS
  ```OCaml
  (compose mu (bimap mu id))((x, y), z)
  ```
- We want to be able to express function equality in point-free notation like this, but it isn't possible just yet
```OCaml
compose mu (bimap id mu) = compose mu (bimap mu id)
```
- Associator - establish Isomorphism between two pairs
```ocaml
let alpha ((x, y), z) = (x, (y, z))
```
- With the help of the associator, we can write this point-free
```OCaml
compose mu (compose (bimap id mu) alpha) = compose mu (bimap mu id)
```
- Moving unit laws to point-free notation. This is the unit law without point-free
```OCaml
mu (eta (), x) = x
mu (x, eta ()) = x
```
- Using bimap
```OCaml
(compose mu (bimap eta id))((), x) = lambda((), x)
(compose mu (bimap id eta))(x, ()) = rho(x, ())
```
- lambda - left unitor and rho - right unitor
```ocaml
let lambda ((), x) = x
```
```ocaml
let rho (x, ()) = x
```
- Point free versions
```OCaml
mu . bimap id eta = rho
mu . bimap eta id = lambda
```
- Associativity and unit laws for cartesian product are only valid upto isomorphism.
- A monoidal category is a category C equipped with a bifunctor called the tensor product `tensor :: C x C -> C` and a distinct object i called the unit object together with three natural isomorphisms - associator, left and right unitors
alpha_{abc} :: (a `tensor` b) `tensor` c -> a `tensor` (b `tensor` c)
lambdaₐ :: i `tensor` a -> a
rhoₐ :: a `tensor` i -> a
### Monoid in a Monoidal Category
- Define monoid in monoidal category
- object m
- Use tensor product to form higher powers of m
- To form a monoid:
  - U :: m `tensor` m
  - N :: i -> m
- tensor product has to be a bifunctor
### Monads as Monoids
- Monads are just monoids in the category of endofunctors
### Monads from Adjunctions
- Adjunction is a pair of functors going back and forth between C and D
- Endofunctors : compose R L and compose L R
- L z = z x s
- R b = s => b
```ocaml
type ('s, 'a) state = State of ('s -> 'a * 's)
```
```ocaml
type ('s, 'a) prod = Prod of 'a * 's
```
```ocaml
type ('s, 'a) reader = Reader of 's -> 'a
```

