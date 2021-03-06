namespace simdjson {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

// clang 6 doesn't think the default constructor can be noexcept, so we make it explicit
simdjson_really_inline field::field() noexcept : std::pair<raw_json_string, ondemand::value>() {}

simdjson_really_inline field::field(raw_json_string key, ondemand::value &&value) noexcept
  : std::pair<raw_json_string, ondemand::value>(key, std::forward<ondemand::value>(value))
{
}

simdjson_really_inline simdjson_result<field> field::start(json_iterator_ref &parent_iter) noexcept {
  raw_json_string key;
  SIMDJSON_TRY( parent_iter->field_key().get(key) );
  SIMDJSON_TRY( parent_iter->field_value() );
  return field::start(parent_iter.borrow(), key);
}

simdjson_really_inline simdjson_result<field> field::start(json_iterator_ref &&iter, raw_json_string key) noexcept {
    return field(key, value::start(std::forward<json_iterator_ref>(iter)));
}

simdjson_really_inline raw_json_string field::key() const noexcept {
  return first;
}

simdjson_really_inline value &field::value() & noexcept {
  return second;
}

simdjson_really_inline value field::value() && noexcept {
  return std::forward<field>(*this).second;
}

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace simdjson

namespace simdjson {

simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::field>::simdjson_result(
  SIMDJSON_IMPLEMENTATION::ondemand::field &&value
) noexcept :
    implementation_simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::field>(
      std::forward<SIMDJSON_IMPLEMENTATION::ondemand::field>(value)
    )
{
}
simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::field>::simdjson_result(
  error_code error
) noexcept :
    implementation_simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::field>(error)
{
}

simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::field>::key() noexcept {
  if (error()) { return error(); }
  return first.key();
}
simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::field>::value() noexcept {
  if (error()) { return error(); }
  return std::move(first.value());
}

} // namespace simdjson
