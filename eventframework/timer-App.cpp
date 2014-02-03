
// This method will be hidden somewhere later
void C_timerevent( void* appC) { static_cast<App*>(appC)->timerevent(); }
void C_startedevent( void* appC) { static_cast<App*>(appC)->started(); }

