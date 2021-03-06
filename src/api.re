let parseFeed = payload =>
  switch (Decoders_bs.Decode.decode_value(Decode_feed.decode_feed, payload)) {
  | Ok(data) => Ok(data)
  | Error(e) => Error(Format.asprintf("%a", Decoders_bs.Decode.pp_error, e))
  };

let feedEndpoint = (~token=?, ~page, user) => {
  let endpoint = {j|https://gh-feed.now.sh/api?user=$(user)&page=$(page)|j};
  switch (token) {
  | None => endpoint
  | Some(token) => {j|$(endpoint)&token=$(token)|j}
  };
};

let fetcher = endpoint =>
  Request.request_json(endpoint)
  ->Promise.flatMapOk(x => Promise.resolved(parseFeed(x)));

module SWR = {
  module SWRConfig = {
    [@bs.module "swr"] [@react.component]
    external make:
      (~value: Js.t({..}), ~children: React.element) => React.element =
      "SWRConfig";
  };

  type t('data, 'error) = {
    data: option('data),
    error: option('error),
  };

  [@bs.module "swr"] external useSWR: string => t('data, 'error) = "default";
};
