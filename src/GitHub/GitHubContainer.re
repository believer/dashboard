type state =
  | Loading
  | Loaded(array(GitHub.notification));

type action =
  | NotificationsFetch
  | NotificationsFetched(array(GitHub.notification));

let component = ReasonReact.reducerComponent("GitHubContainer");

let make = _children => {
  ...component,

  didMount: self => {
    self.send(NotificationsFetch);

    let intervalId =
      Js.Global.setInterval(() => self.send(NotificationsFetch), 1000 * 60);
    self.onUnmount(() => Js.Global.clearInterval(intervalId));
  },

  initialState: () => Loading,

  reducer: (action: action, _state: state) =>
    switch (action) {
    | NotificationsFetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              GitHub.getNotifications()
              |> then_(notifications =>
                   self.send(NotificationsFetched(notifications)) |> resolve
                 )
              |> ignore
            )
        ),
      )
    | NotificationsFetched(notifications) =>
      ReasonReact.Update(Loaded(notifications))
    },

  render: ({state}) =>
    <div className="w-100 w-50-l mr4 mb4">
      <Header color="b--blue" title="GitHub" />
      <Card>
        {
          Storage.hasConfig("github") ?
            switch (state) {
            | Loading =>
              <div className="tc light-silver"> {"Loading" |> Utils.str} </div>
            | Loaded(notifications) =>
              Array.length(notifications) > 0 ?
                notifications
                |> Array.mapi((i, item: GitHub.notification) =>
                     <GitHubNotification
                       item
                       isLast={Array.length(notifications) - 1 === i}
                       key={item.id}
                     />
                   )
                |> ReasonReact.array :
                <EmptyState />
            } :
            <GitHubMissing />
        }
      </Card>
    </div>,
};
