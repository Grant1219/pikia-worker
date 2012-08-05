function handler (jobContext)
    print ('Inside lua job handler!')
    return true
end

dispatcher:register_handler (1, handler)
