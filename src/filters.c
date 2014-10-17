#include "filters.h"
#include "repo.h"

enum Error filters_init(void)
{
	git_config_set_string(repo_config_handle,
	                      "filter.big.clean", "git-big filter-clean");
	git_config_set_string(repo_config_handle,
	                      "filter.big.smudge", "git-big filter-smudge");

	// FIXME: need to find a way to make these filters required...

	return ERROR_NONE;
}
