uint64 MANIPULATION_POSE_UNREACHABLE = 64
uint64 GRASP_FAILED = 128  # Grasp into the void
uint64 OBJECT_NOT_FOUND = 256
uint64 VISION_PRIMITIVE_FAILED = 512
uint64 CONTRADICTING_TACTILE_FEEDBACK = 1024  # Collide without expecting it
uint64 CONTRADICTING_VISION_RESULTS = 2048
uint64 GRASP_FAILED_AND_CRASHED = 4096  # Throwing something out of the way
uint64 JLO_ERROR = 8192  # Could not get position
uint64 VECTOR_FIELD_CANT_REACH = 16384  # The arm got stuck along the way, did not reach the final grasping pose

uint64  error_id                # One of the error constants defined above
string  node_name               # The node causing this error
string  error_description       # Further information about the error
