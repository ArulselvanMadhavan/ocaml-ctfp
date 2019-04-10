# Products and Coproducts
* Universal Construction
  - Defining objects in terms of their relationships.
* Initial Object
  - The object that has one and only arrow to any object in the category.
  - This object is unique upto isomorphism.
* Absurd definition
```ocaml
# type void
```
```ocaml
# let rec absurd (x:void) = absurd x
```
* Terminal Object
