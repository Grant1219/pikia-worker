function handler (jobContext)
    print ('Lua handler for job: ' .. jobContext.id)
    print ('Read int: ' .. jobContext.buf:read_int32 () )
    print ('Read string: ' .. jobContext.buf:read_string () )
    return true
end

dispatcher:register_handler (1, handler)
