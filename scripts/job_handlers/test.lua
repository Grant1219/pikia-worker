function handler (jobContext)
    print ('Lua handler for job: ' .. jobContext.id)
    return true
end

script_manager:register_handler (1, handler)
