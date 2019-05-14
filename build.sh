#!/bin/sh
# FIXME: The order of these will likely need to be revised.

((cd freakazoid && make) & \
(cd loadpng && make) & \
(cd Utility/util && make)) && \
(cd OmfgScript && make) && \
(cd Console && make) && \
(cd http && make) && \
(cd lua51 && make) && \
(cd GUI/detail && make) && \
(cd Goop/blitters && make) && \
(cd lua51/luaapi && make) && \
(cd GUI/lua && make) && \
(cd Goop/lua && make) && \
(cd Goop && make) && \
true
