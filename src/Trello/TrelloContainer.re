type state =
  | Loading
  | Loaded(array(Trello.notification));

type action =
  | NotificationsFetch
  | NotificationsFetched(array(Trello.notification));

let component = ReasonReact.reducerComponent("TrelloContainer");

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
              Trello.getNotifications()
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

  render: ({state}) => {
    let hasTrelloConfig =
      Storage.hasConfig("trello_username")
      && Storage.hasConfig("trello_key")
      && Storage.hasConfig("trello_token");

    <div className="w-100 w-50-l">
      <Header color="b--light-red" title="Trello" />
      <Card>
        {
          hasTrelloConfig ?
            switch (state) {
            | Loading =>
              <div className="tc light-silver"> {"Loading" |> Utils.str} </div>
            | Loaded(notifications) =>
              Array.length(notifications) > 0 ?
                notifications
                |> Array.mapi((i, item: Trello.notification) =>
                     <TrelloNotification
                       item
                       isLast={Array.length(notifications) - 1 === i}
                       key={item.id}
                     />
                   )
                |> ReasonReact.array :
                <EmptyState />
            } :
            <TrelloMissing />
        }
      </Card>
    </div>;
  },
};
