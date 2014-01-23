#Message that cop uses to  answer on specified topics, U. Klank klank@in.tum.de
uint64         perception_primitive         # Perception primitive that caused this answer
float64        evaluation                   # A score describing the quality of the outcomes of the perception primitive
system_error[] error                        # A list of occured system failures which might be caused by the perception primitive
uint64[]       eval_whitelist               # list results which are included into the evaluation (note: default is all, not none, so an empty list means that all elements have to be evaluated, since the case that none of the results should get an evaluation does not make much sense)
