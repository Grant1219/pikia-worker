function handler (jobContext)
    print ('Inside lua job handler!')
end

dispatcher:register_handler (1, handler)
