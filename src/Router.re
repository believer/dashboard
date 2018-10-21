type route =
  | Home
  | Settings;

type state = {route};

type action =
  | ChangeRoute(route);

let component = ReasonReact.reducerComponent("Router");

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.hash) {
  | "" => Home
  | "/settings" => Settings
  | _ => Home
  };

let make = _children => {
  ...component,

  initialState: () => {
    route: ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute,
  },

  reducer: (action, _state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update({route: route})
    },

  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url =>
        self.send(ChangeRoute(url |> mapUrlToRoute))
      );

    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: ({state}) =>
    <div className="flex h-100">
      <Navigation />
      {
        switch (state.route) {
        | Home => <Home />
        | Settings => <Settings />
        }
      }
    </div>,
};
