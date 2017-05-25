include(../cmake/qtest.cmake)
set(TESTNAME QmgObservableVariantListProxyTest)
set(SRC ${TESTNAME}.cpp)
set(NEEDED_FEATURES
    cxx_auto_type
    cxx_lambdas
	cxx_range_for
	cxx_final)
include(../cmake/base.cmake)
