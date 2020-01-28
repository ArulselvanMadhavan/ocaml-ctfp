# Kan Extensions
### Utilities used by code below
```ocaml
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
module type Monoid = sig
  type m
  val mempty : m
  val mappend : m -> m -> m
end
module ListMonoid(T1 : sig type a end) : (Monoid with type m = T1.a list) = struct
  type m = T1.a list
  let mempty = []
  let mappend xs ys = List.append xs ys
end
```
### Kan
```ocaml
module type Ran = sig
  type 'a k
  type 'a d
  type 'a ran = Ran of { r : 'i. ('a -> 'i k) -> 'i d }
end
```
```OCaml
val f : string -> int tree
```
```OCaml
(* Higher rank polymorphic functions can be achieved using records *)
{ r : 'i. (a -> 'i k) -> 'i }
```
```ocaml
module type Lst = sig
  type a
  type m
  module M : Monoid with type m = m
  type lst = (a -> M.m) -> M.m
  val f : lst
end
```
```ocaml
let fold_map (type i) (module M : Monoid with type m = i) xs f = List.fold_left (fun acc -> fun a -> M.mappend acc (f a)) M.mempty xs

let to_lst (type x) (xs : x list) =
  let module LM : Monoid with type m = x list = ListMonoid(struct type a = x end) in
  (module struct
     type a = x 
     type m = x list
     module M = LM
     type lst = (a -> LM.m) -> LM.m
     let f = fun g -> fold_map (module LM) xs g 
   end : Lst with type a = x)

let from_lst (type x) (module LstImpl : Lst with type a = x and type m = x list) =
  LstImpl.f (fun x -> [x])
```
```ocaml
module type Lan = sig
  type 'a k
  type 'a d
  type a
  type i
  val fk : i k -> a
  val di : i d
end
```
```ocaml
module type Exp = sig
  type a
  type b
  type 'a d = I of 'a
  type 'a k = ('a * a)
  include Lan with type 'a k := a * 'a and type 'a d := 'a d and type a := b
end
```
```ocaml
let to_exp (type a') (type b') = fun f ->
  (module struct
     type a = a'
     type b = b'
     type 'a d = I of 'a
     type 'a k = ('a * a)
     type i = unit
     let fk = fun (a, _) -> f a
     let di = I ()
   end : Exp with type a = a' and type b = b')
   
let from_exp (type a') (type b') (module E : Exp with type a = a' and type b = b') = fun a -> 
  let (I i) = E.di in
  E.fk (a, i)
```
### Free Functor
```ocaml
module type FreeF = sig
  type 'a f
  type a
  type i
  val h : i -> a
  val fi : i -> i f
end
```
```ocaml
module FreeFunctor(F : sig type 'a f end) : Functor = struct
  module type F = FreeF with type 'a f = 'a F.f
  type 'a t = (module F with type a = 'a)
  
  let fmap (type a') (type b') (f : a' -> b') = fun (module FF : F with type a = a') -> (module struct
  type 'a f = 'a F.f
  type a = b'
  type i = FF.i
  let h = fun i -> f (FF.h i)
  let fi = FF.fi
  end : F with type a = b')
  
end
```
```ocaml
module type FreeF_Alt = sig
  type a
  type 'a f
  val freeF : (a -> 'i) -> 'i f 
end
```
```ocaml
module FreeFunctorAlt(F : sig type 'a f end) : Functor = struct
  module type F = FreeF_Alt with type 'a f = 'a F.f
  type 'a t = (module F with type a = 'a)
  
  let fmap (type a') (type b')  f = fun (module FF : F with type a = a') ->
    (module struct
       type a = b'
       type 'a f = 'a F.f
       let freeF = fun bi -> 
         FF.freeF (fun a -> bi (f a))
    end : F with type a = b')
end
```
