type state =
  | Error(string)
  | Loading
  | Loaded(array(Trello.notification));

type action =
  | NotificationsError(string)
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
    | NotificationsError(err) => ReasonReact.Update(Error(err))
    | NotificationsFetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Trello.getNotifications()
              |> then_(notifications => {
                   DocumentTitle.updateTitleWithNotifications(
                     GitHub.Config.numberOfNotifications,
                     Trello.Config.numberOfNotifications,
                   );
                   self.send(NotificationsFetched(notifications)) |> resolve;
                 })
              |> catch(_ => self.send(NotificationsError("err")) |> resolve)
              |> ignore
            )
        ),
      )
    | NotificationsFetched(notifications) =>
      ReasonReact.Update(Loaded(notifications))
    },

  render: ({state}) =>
    <div className="w-100 w-50-l">
      <Header color="b--light-red" title="Trello" />
      <Card>
        {
          Trello.Config.hasConfig ?
            switch (state) {
            | Error(_) =>
              <div className="tc light-red">
                {
                  "Something went wrong. Are the settings up to date?"
                  |> Utils.str
                }
              </div>
            | Loading =>
              <div className="tc mid-gray"> {"Loading" |> Utils.str} </div>
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
    </div>,
};
